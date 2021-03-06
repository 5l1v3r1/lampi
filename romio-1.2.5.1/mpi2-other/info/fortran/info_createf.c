/* 
 *   $Id$    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpio.h"
#include "adio.h"





#if defined(HAVE_WEAK_SYMBOLS) && defined(FORTRANUNDERSCORE) 

void pmpi_info_create(void);
void mpi_info_create(void);
/*  void pmpi_info_create_(void);   this is the real function, below */
void mpi_info_create_(void);   
void pmpi_info_create__(void);
void mpi_info_create__(void);
void PMPI_INFO_CREATE(void);
void MPI_INFO_CREATE(void);

#pragma weak PMPI_INFO_CREATE = pmpi_info_create_     
#pragma weak pmpi_info_create = pmpi_info_create_
#pragma weak pmpi_info_create__ = pmpi_info_create_
#pragma weak MPI_INFO_CREATE = pmpi_info_create_     
#pragma weak mpi_info_create = pmpi_info_create_
#pragma weak mpi_info_create_ = pmpi_info_create_  
#pragma weak mpi_info_create__ = pmpi_info_create_
#endif



#if defined(MPIO_BUILD_PROFILING) || defined(HAVE_WEAK_SYMBOLS)
#ifdef FORTRANCAPS
#define mpi_info_create_ PMPI_INFO_CREATE
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_info_create_ pmpi_info_create__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_info_create pmpi_info_create_
#endif
#define mpi_info_create_ pmpi_info_create
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_info_create_ pmpi_info_create
#endif
#define mpi_info_create_ pmpi_info_create_
#endif

#if defined(HAVE_WEAK_SYMBOLS)
#if defined(HAVE_PRAGMA_WEAK)
#if defined(FORTRANCAPS)
#pragma weak MPI_INFO_CREATE = PMPI_INFO_CREATE
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma weak mpi_info_create__ = pmpi_info_create__
#elif !defined(FORTRANUNDERSCORE)
#pragma weak mpi_info_create = pmpi_info_create
#else
//#pragma weak mpi_info_create_ = pmpi_info_create_
#endif

#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#if defined(FORTRANCAPS)
#pragma _HP_SECONDARY_DEF PMPI_INFO_CREATE MPI_INFO_CREATE
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_info_create__ mpi_info_create__
#elif !defined(FORTRANUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_info_create mpi_info_create
#else
#pragma _HP_SECONDARY_DEF pmpi_info_create_ mpi_info_create_
#endif

#elif defined(HAVE_PRAGMA_CRI_DUP)
#if defined(FORTRANCAPS)
#pragma _CRI duplicate MPI_INFO_CREATE as PMPI_INFO_CREATE
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _CRI duplicate mpi_info_create__ as pmpi_info_create__
#elif !defined(FORTRANUNDERSCORE)
#pragma _CRI duplicate mpi_info_create as pmpi_info_create
#else
#pragma _CRI duplicate mpi_info_create_ as pmpi_info_create_
#endif

/* end of weak pragmas */
#endif
/* Include mapping from MPI->PMPI */
#include "mpioprof.h"
#endif

#else

#ifdef FORTRANCAPS
#define mpi_info_create_ MPI_INFO_CREATE
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_info_create_ mpi_info_create__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_info_create mpi_info_create_
#endif
#define mpi_info_create_ mpi_info_create
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_info_create_ mpi_info_create
#endif
#endif
#endif

void mpi_info_create_(MPI_Fint *info, int *ierr )
{
    MPI_Info info_c;

    *ierr = MPI_Info_create(&info_c);
    *info = MPI_Info_c2f(info_c);
}
