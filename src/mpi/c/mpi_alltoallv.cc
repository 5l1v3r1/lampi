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



#include "internal/mpi.h"
#include "queue/Communicator.h"
#include "queue/globals.h"

#pragma weak MPI_Alltoallv = PMPI_Alltoallv
int PMPI_Alltoallv(void *sendbuf, int *sendcounts, int *senddispls,
		   MPI_Datatype senddatatype,
		   void *recvbuf, int *recvcounts, int *recvdispls,
		   MPI_Datatype recvdatatype, MPI_Comm comm)
{
    int rc;
    ULMType_t *sendtype;
    ULMType_t *recvtype;
    Communicator *communicator;

    if (_mpi.check_args) {
        rc = MPI_SUCCESS;
        if (_mpi.finalized) {
            rc = MPI_ERR_INTERN;
        } else if (sendbuf == NULL) {
            rc = MPI_ERR_BUFFER;
        } else if (senddatatype == MPI_DATATYPE_NULL) {
            rc = MPI_ERR_TYPE;
        } else if (sendcounts == NULL) {
            rc = MPI_ERR_COUNT;
        } else if (senddispls == NULL) {
            rc = MPI_ERR_DISP;
        } else if (recvbuf == NULL) {
            rc = MPI_ERR_BUFFER;
        } else if (recvdatatype == MPI_DATATYPE_NULL) {
            rc = MPI_ERR_TYPE;
        } else if (recvcounts == NULL) {
            rc = MPI_ERR_COUNT;
        } else if (recvdispls == NULL) {
            rc = MPI_ERR_DISP;
        } else if (ulm_invalid_comm(comm)) {
            rc = MPI_ERR_COMM;
        }
        if (rc != MPI_SUCCESS) {
            goto ERRHANDLER;
        }
    }

    sendtype = (ULMType_t *) senddatatype;
    recvtype = (ULMType_t *) recvdatatype;
    communicator = communicators[comm];
    rc = communicator->collective.alltoallv(sendbuf, sendcounts, senddispls,
                                   (ULMType_t *)sendtype, recvbuf, recvcounts,
                                   recvdispls, (ULMType_t *)recvtype, comm);
    rc = (rc == ULM_SUCCESS) ? MPI_SUCCESS : _mpi_error(rc);

ERRHANDLER:
    if (rc != MPI_SUCCESS) {
	_mpi_errhandler(comm, rc, __FILE__, __LINE__);
    }

    return rc;
}