#include <R.h>
#include <Rinternals.h>

#include "backend.h"

/* 
  PRIORITY (highest to lowest):
    RTH_NUM_THREADS
    OMP_NUM_THREADS (for OpenMP backends)
    # cores
*/

#define RTH_ERROR -2147483648 // int NA in R

extern "C" SEXP Rth_get_backend()
{
  SEXP backend;
  PROTECT(backend = allocVector(STRSXP, 1));
  
  #if RTH_OMP
  SET_STRING_ELT(backend, 0, mkChar("omp"));
  #elif RTH_TBB
  SET_STRING_ELT(backend, 0, mkChar("tbb"));
  #elif RTH_CUDA
  SET_STRING_ELT(backend, 0, mkChar("cuda"));
  #else
  SET_STRING_ELT(backend, 0, mkChar("unknown"));
  #endif
  
  UNPROTECT(1);
  return backend;
}



extern "C" SEXP Rth_get_num_threads()
{
  char *rth_nthreads = getenv("RTH_NUM_THREADS");
  char *omp_nthreads = getenv("OMP_NUM_THREADS");
  
  SEXP nthreads;
  PROTECT(nthreads = allocVector(INTSXP, 1));
  
  #if RTH_OMP
  if (rth_nthreads != NULL)
    INT(nthreads) = atoi(rth_nthreads);
  else if (omp_nthreads != NULL)
    INT(nthreads) = atoi(omp_nthreads);
  else
    INT(nthreads) = omp_get_max_threads(); // omp_get_max_threads
  #elif RTH_TBB
  if (rth_nthreads != NULL)
    INT(nthreads) = atoi(rth_nthreads);
  else
    INT(nthreads) = tbb::task_scheduler_init::automatic;
  #else
  INT(nthreads) = RTH_ERROR;
  #endif
  
  
  UNPROTECT(1);
  return nthreads;
}

