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

#pragma weak MPI_Cart_rank = PMPI_Cart_rank
int PMPI_Cart_rank(MPI_Comm comm, int *coords, int *rank)
{
    ULMTopology_t *topology;
    int factor;
    int i;
    int rc;

    if (coords == NULL || rank == NULL) {
	rc = MPI_ERR_ARG;
	goto ERRHANDLER;
    }

    rc = ulm_get_topology(comm, &topology);
    if (rc != ULM_SUCCESS) {
	rc = MPI_ERR_COMM;
	goto ERRHANDLER;
    }
    if ((topology == NULL) || topology->type != MPI_CART) {
	rc = MPI_ERR_TOPOLOGY;
	goto ERRHANDLER;
    }

    factor = 1;
    *rank = 0;
    for (i = topology->cart.ndims - 1; i >= 0; i--) {

        int coord = coords[i];
        int size = topology->cart.dims[i];

	if (topology->cart.periods[i]) {
	    coord = coord % size;
	    if (coord < 0) {
		coord += size;
	    }
	} else {
	    if (coord < 0 || coord > size) {
                /* check to see if this is MPI_PROC_NULL */
                if( coord == MPI_PROC_NULL ){
                    rc = MPI_SUCCESS;
                    *rank=MPI_PROC_NULL;
                } else {
                    rc = MPI_ERR_DIMS;
                }
                goto ERRHANDLER;
	    }
	}

        *rank += coord * factor;
	factor *= size;
    }

    rc = MPI_SUCCESS;

ERRHANDLER:
    if (rc != MPI_SUCCESS) {
	_mpi_errhandler(comm, rc, __FILE__, __LINE__);
    }

    return rc;
}
