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



#include <stdlib.h>
#include <string.h>

#include "init/environ.h"
#include "internal/mpi.h"
#include "internal/buffer.h"
#include "internal/cLock.h"
#include "internal/state.h"

/*
 * MPI state
 */
mpi_state_t _mpi;
static int called=0;

/*
 * Something for the proc null request handle to point at
 */
static int dummy_proc_null_request;

/*
 * MPI layer initialization
 */
int _mpi_init(void)
{
    int 		rc, val;

    if (_MPI_DEBUG) {
	_mpi_dbg("_mpi_init:\n");
    }

    lampi_environ_find_integer("LAMPI_NO_CHECK_ARGS", &val);
    if ( 1 == val ) {
        _mpi.check_args = 0;
    } else {
        _mpi.check_args = lampiState.checkargs;
    }
    _mpi.threadsafe = 0;
    /* initialize _mpi lock - upper layer is responsible to ensure that
     * only one thread at a time makes a call
     */
    if( !called) {
        called=1;
        cLockInit(&(_mpi.lock));
    }
    _mpi_lock(&(_mpi.lock));

    if (_mpi.initialized == 0) {

	if (_MPI_DEBUG) {
	    _mpi_dbg("_mpi_init: initializing\n");
	}

	_mpi.initialized = 1;
	_mpi.finalized = 0;
	_mpi.proc_null_request = &dummy_proc_null_request;

        rc = _mpi_init_collectives();
        if (rc < 0) {
            return rc;
        }

	rc = _mpi_init_datatypes();
	if (rc < 0) {
	    return rc;
	}

	rc = _mpi_init_operations();
	if (rc < 0) {
	    return rc;
	}

	/*
	 * dynamic table of pointer to error handler structs
	 */
	_mpi.errhandler_table = _mpi_create_errhandler_table();
	if (_mpi.errhandler_table == NULL) {
	    return -1;
	}

	/*
	 * dynamic table of pointers which can be freed when there are
	 * no pending messages
	 */
	_mpi.free_table = ulm_malloc(sizeof(ptr_table_t));
	if (_mpi.free_table == NULL) {
	    return -1;
	}
        memset(_mpi.free_table, 0, sizeof(ptr_table_t));
	cLockInit(&(_mpi.free_table->lock));
    }

    _mpi_unlock(&(_mpi.lock));

    /*
     * initialize the control structure of buffered sends
     */
    lampiState.bsendData->poolInUse = 0;
    lampiState.bsendData->bufferLength = 0;
    lampiState.bsendData->bytesInUse = 0;
    lampiState.bsendData->buffer = 0;
    lampiState.bsendData->allocations = NULL;
    cLockInit(&(lampiState.bsendData->Lock));

    return MPI_SUCCESS;
}
