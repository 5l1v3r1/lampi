/* 
 *   $Id$    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpio.h"
#include "adio.h"





#if defined(HAVE_WEAK_SYMBOLS) && defined(FORTRANUNDERSCORE) 

void pmpi_file_set_info(void);
void mpi_file_set_info(void);
/*  void pmpi_file_set_info_(void);   this is the real function, below */
void mpi_file_set_info_(void);   
void pmpi_file_set_info__(void);
void mpi_file_set_info__(void);
void PMPI_FILE_SET_INFO(void);
void MPI_FILE_SET_INFO(void);

#pragma weak PMPI_FILE_SET_INFO = pmpi_file_set_info_     
#pragma weak pmpi_file_set_info = pmpi_file_set_info_
#pragma weak pmpi_file_set_info__ = pmpi_file_set_info_
#pragma weak MPI_FILE_SET_INFO = pmpi_file_set_info_     
#pragma weak mpi_file_set_info = pmpi_file_set_info_
#pragma weak mpi_file_set_info_ = pmpi_file_set_info_  
#pragma weak mpi_file_set_info__ = pmpi_file_set_info_
#endif



#if defined(MPIO_BUILD_PROFILING) || defined(HAVE_WEAK_SYMBOLS)
#ifdef FORTRANCAPS
#define mpi_file_set_info_ PMPI_FILE_SET_INFO
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_set_info_ pmpi_file_set_info__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_file_set_info pmpi_file_set_info_
#endif
#define mpi_file_set_info_ pmpi_file_set_info
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_file_set_info_ pmpi_file_set_info
#endif
#define mpi_file_set_info_ pmpi_file_set_info_
#endif

#if defined(HAVE_WEAK_SYMBOLS)
#if defined(HAVE_PRAGMA_WEAK)
#if defined(FORTRANCAPS)
#pragma weak MPI_FILE_SET_INFO = PMPI_FILE_SET_INFO
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma weak mpi_file_set_info__ = pmpi_file_set_info__
#elif !defined(FORTRANUNDERSCORE)
#pragma weak mpi_file_set_info = pmpi_file_set_info
#else
//#pragma weak mpi_file_set_info_ = pmpi_file_set_info_
#endif

#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#if defined(FORTRANCAPS)
#pragma _HP_SECONDARY_DEF PMPI_FILE_SET_INFO MPI_FILE_SET_INFO
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_set_info__ mpi_file_set_info__
#elif !defined(FORTRANUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_set_info mpi_file_set_info
#else
#pragma _HP_SECONDARY_DEF pmpi_file_set_info_ mpi_file_set_info_
#endif

#elif defined(HAVE_PRAGMA_CRI_DUP)
#if defined(FORTRANCAPS)
#pragma _CRI duplicate MPI_FILE_SET_INFO as PMPI_FILE_SET_INFO
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _CRI duplicate mpi_file_set_info__ as pmpi_file_set_info__
#elif !defined(FORTRANUNDERSCORE)
#pragma _CRI duplicate mpi_file_set_info as pmpi_file_set_info
#else
#pragma _CRI duplicate mpi_file_set_info_ as pmpi_file_set_info_
#endif

/* end of weak pragmas */
#endif
/* Include mapping from MPI->PMPI */
#include "mpioprof.h"
#endif

#else

#ifdef FORTRANCAPS
#define mpi_file_set_info_ MPI_FILE_SET_INFO
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_set_info_ mpi_file_set_info__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_file_set_info mpi_file_set_info_
#endif
#define mpi_file_set_info_ mpi_file_set_info
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_file_set_info_ mpi_file_set_info
#endif
#endif
#endif

/* Prototype to keep compiler happy */
FORTRAN_API void FORT_CALL mpi_file_set_info_(MPI_Fint *fh, MPI_Fint *info, int *ierr );

FORTRAN_API void FORT_CALL mpi_file_set_info_(MPI_Fint *fh, MPI_Fint *info, int *ierr )
{
    MPI_File fh_c;
    MPI_Info info_c;
    
    fh_c = MPI_File_f2c(*fh);
    info_c = MPI_Info_f2c(*info);

    *ierr = MPI_File_set_info(fh_c, info_c);
}