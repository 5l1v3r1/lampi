/* 
 *   $Id$    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpio.h"
#include "adio.h"





#if defined(HAVE_WEAK_SYMBOLS) && defined(FORTRANUNDERSCORE) 

void pmpi_file_read_ordered(void);
void mpi_file_read_ordered(void);
/*  void pmpi_file_read_ordered_(void);   this is the real function, below */
void mpi_file_read_ordered_(void);   
void pmpi_file_read_ordered__(void);
void mpi_file_read_ordered__(void);
void PMPI_FILE_READ_ORDERED(void);
void MPI_FILE_READ_ORDERED(void);

#pragma weak PMPI_FILE_READ_ORDERED = pmpi_file_read_ordered_     
#pragma weak pmpi_file_read_ordered = pmpi_file_read_ordered_
#pragma weak pmpi_file_read_ordered__ = pmpi_file_read_ordered_
#pragma weak MPI_FILE_READ_ORDERED = pmpi_file_read_ordered_     
#pragma weak mpi_file_read_ordered = pmpi_file_read_ordered_
#pragma weak mpi_file_read_ordered_ = pmpi_file_read_ordered_  
#pragma weak mpi_file_read_ordered__ = pmpi_file_read_ordered_
#endif



#if defined(MPIO_BUILD_PROFILING) || defined(HAVE_WEAK_SYMBOLS)
#ifdef FORTRANCAPS
#define mpi_file_read_ordered_ PMPI_FILE_READ_ORDERED
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_read_ordered_ pmpi_file_read_ordered__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_file_read_ordered pmpi_file_read_ordered_
#endif
#define mpi_file_read_ordered_ pmpi_file_read_ordered
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_file_read_ordered_ pmpi_file_read_ordered
#endif
#define mpi_file_read_ordered_ pmpi_file_read_ordered_
#endif

#if defined(HAVE_WEAK_SYMBOLS)
#if defined(HAVE_PRAGMA_WEAK)
#if defined(FORTRANCAPS)
#pragma weak MPI_FILE_READ_ORDERED = PMPI_FILE_READ_ORDERED
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma weak mpi_file_read_ordered__ = pmpi_file_read_ordered__
#elif !defined(FORTRANUNDERSCORE)
#pragma weak mpi_file_read_ordered = pmpi_file_read_ordered
#else
//#pragma weak mpi_file_read_ordered_ = pmpi_file_read_ordered_
#endif

#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#if defined(FORTRANCAPS)
#pragma _HP_SECONDARY_DEF PMPI_FILE_READ_ORDERED MPI_FILE_READ_ORDERED
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_read_ordered__ mpi_file_read_ordered__
#elif !defined(FORTRANUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_read_ordered mpi_file_read_ordered
#else
#pragma _HP_SECONDARY_DEF pmpi_file_read_ordered_ mpi_file_read_ordered_
#endif

#elif defined(HAVE_PRAGMA_CRI_DUP)
#if defined(FORTRANCAPS)
#pragma _CRI duplicate MPI_FILE_READ_ORDERED as PMPI_FILE_READ_ORDERED
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _CRI duplicate mpi_file_read_ordered__ as pmpi_file_read_ordered__
#elif !defined(FORTRANUNDERSCORE)
#pragma _CRI duplicate mpi_file_read_ordered as pmpi_file_read_ordered
#else
#pragma _CRI duplicate mpi_file_read_ordered_ as pmpi_file_read_ordered_
#endif

/* end of weak pragmas */
#endif
/* Include mapping from MPI->PMPI */
#include "mpioprof.h"
#endif

#else

#ifdef FORTRANCAPS
#define mpi_file_read_ordered_ MPI_FILE_READ_ORDERED
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_read_ordered_ mpi_file_read_ordered__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_file_read_ordered mpi_file_read_ordered_
#endif
#define mpi_file_read_ordered_ mpi_file_read_ordered
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_file_read_ordered_ mpi_file_read_ordered
#endif
#endif
#endif

#if defined(MPIHP) || defined(MPILAM) || defined(MPILAMPI)
/* Prototype to keep compiler happy */
void mpi_file_read_ordered_(MPI_Fint *fh,void *buf,int *count,
		    MPI_Fint *datatype,MPI_Status *status, int *ierr );

void mpi_file_read_ordered_(MPI_Fint *fh,void *buf,int *count,
                      MPI_Fint *datatype,MPI_Status *status, int *ierr )
{
    MPI_File fh_c;
    MPI_Datatype datatype_c;
    
    fh_c = MPI_File_f2c(*fh);
    datatype_c = MPI_Type_f2c(*datatype);

    *ierr = MPI_File_read_ordered(fh_c,buf,*count,datatype_c,status);
}
#else
/* Prototype to keep compiler happy */
FORTRAN_API void FORT_CALL mpi_file_read_ordered_(MPI_Fint *fh,void *buf,int *count,
		    MPI_Datatype *datatype,MPI_Status *status, int *ierr );

FORTRAN_API void FORT_CALL mpi_file_read_ordered_(MPI_Fint *fh,void *buf,int *count,
                      MPI_Datatype *datatype,MPI_Status *status, int *ierr ){
    MPI_File fh_c;
    
    fh_c = MPI_File_f2c(*fh);
    *ierr = MPI_File_read_ordered(fh_c,buf,*count,*datatype,status);
}
#endif