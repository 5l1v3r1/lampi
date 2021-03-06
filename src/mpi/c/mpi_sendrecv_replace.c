/*
 * Copyright 2002-2003. The Regents of the University of California. This material 
 * was produced under U.S. Government contract W-7405-ENG-36 for Los Alamos 
 * National Laboratory, which is operated by the University of California for 
 * the U.S. Department of Energy. The Government is granted for itself and 
 * others acting on its behalf a paid-up, nonexclusive, irrevocable worldwide 
 * license in this material to reproduce, prepare derivative works, and 
 * perform publicly and display publicly. Beginning five (5) years after 
 * October 10,2002 subject to additional five-year worldwide renewals, the 
 * Government is granted for itself and others acting on its behalf a paid-up, 
 * nonexclusive, irrevocable worldwide license in this material to reproduce, 
 * prepare derivative works, distribute copies to the public, perform publicly 
 * and display publicly, and to permit others to do so. NEITHER THE UNITED 
 * STATES NOR THE UNITED STATES DEPARTMENT OF ENERGY, NOR THE UNIVERSITY OF 
 * CALIFORNIA, NOR ANY OF THEIR EMPLOYEES, MAKES ANY WARRANTY, EXPRESS OR 
 * IMPLIED, OR ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY, 
 * COMPLETENESS, OR USEFULNESS OF ANY INFORMATION, APPARATUS, PRODUCT, OR 
 * PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT INFRINGE PRIVATELY 
 * OWNED RIGHTS.

 * Additionally, this program is free software; you can distribute it and/or 
 * modify it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; either version 2 of the License, 
 * or any later version.  Accordingly, this program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Lesser General Public License for more details.
 */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "internal/mpi.h"
#include "internal/type_copy.h"
#include "internal/malloc.h"

#ifdef HAVE_PRAGMA_WEAK
#pragma weak MPI_Sendrecv_replace = PMPI_Sendrecv_replace
#endif

int PMPI_Sendrecv_replace(void *buf, int count, MPI_Datatype mtype,
			  int dest, int sendtag, int source, int recvtag,
			  MPI_Comm comm, MPI_Status *status)
{
    enum {
        KBYTE = 1 << 10,
        BLOCK_SIZE = 2 * KBYTE
    };

    int rc;

    if (_mpi.check_args) {
        rc = MPI_SUCCESS;
        if (_mpi.finalized) {
            rc = MPI_ERR_INTERN;
        } else if (ulm_invalid_comm(comm)) {
            rc = MPI_ERR_COMM;
        } else if (count < 0) {
            rc = MPI_ERR_COUNT;
        } else if (mtype == MPI_DATATYPE_NULL) {
            rc = MPI_ERR_TYPE;
        } else if (ulm_invalid_destination(comm, dest)) {
            rc = MPI_ERR_RANK;
        } else if (sendtag < 0 || sendtag > MPI_TAG_UB_VALUE) {
            rc = MPI_ERR_TAG;
        } else if (ulm_invalid_source(comm, source)) {
            rc = MPI_ERR_RANK;
        } else if (recvtag < MPI_ANY_TAG || recvtag > MPI_TAG_UB_VALUE) {
            rc = MPI_ERR_TAG;
        }
        if (rc != MPI_SUCCESS) {
            goto ERRHANDLER;
        }
    }

    if (source == MPI_PROC_NULL || dest == MPI_PROC_NULL) {

        /*
         * for null source/destination, no problem with buffer reuse,
         * so just call MPI_Sendrecv
         */

        rc = MPI_Sendrecv(buf, count, mtype, dest, sendtag,
                          buf, count, mtype, source, recvtag,
                          comm, status);

    } else {

        MPI_Status status_tmp;
        ULMType_t *type = mtype;
        size_t type_index = 0;
        size_t map_index = 0;
        size_t map_offset = 0;
        size_t recv_offset = 0;
        size_t recv_size = count * type->packed_size;
        unsigned char recv_block[BLOCK_SIZE];
        unsigned char *recv_buf = recv_block;
 
        /* if required - allocate a buffer for the receive */
        if(recv_size > BLOCK_SIZE) {
            recv_buf = ulm_malloc(recv_size);
            if(recv_buf == NULL) {
                rc = MPI_ERR_NO_MEM;
                goto ERRHANDLER;
            }
        }

        /* we need a real status so that we can unpack */
        if (status == MPI_STATUS_IGNORE) {
            status = &status_tmp;
        }

        rc = MPI_Sendrecv(buf, count, mtype, dest, sendtag,
                          recv_buf, recv_size, MPI_BYTE, source, recvtag,
                          comm, status);
        if (rc != MPI_SUCCESS) {
            if(recv_buf != recv_block)
                ulm_free(recv_buf);
            goto ERRHANDLER;
        }

        /* unpack the recv buffer into original buffer */
        rc = type_pack(TYPE_PACK_UNPACK,
                       recv_buf, status->_count, &recv_offset,
                       buf, count, mtype,
                       &type_index,
                       &map_index,
                       &map_offset);

        if(recv_buf != recv_block) {
            ulm_free(recv_buf);
        }
        
        return MPI_SUCCESS;
    }

ERRHANDLER:
    if (rc != MPI_SUCCESS) {
	_mpi_errhandler(comm, rc, __FILE__, __LINE__);
    }

    return rc;
}
