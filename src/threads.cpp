#include <R.h>
#include <Rinternals.h>

#include "backend.h"

/* 
  PRIORITY (highest to lowest):
    RTH_NUM_THREADS
    OMP_NUM_THREADS (for OpenMP backends)
    # cores
*/

extern "C" {

SEXP Rth_get_backend()
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



SEXP Rth_omp_get_num_threads()
{
  char *rth_nthreads = getenv("RTH_NUM_THREADS");
  char *omp_nthreads = getenv("OMP_NUM_THREADS");
  
  SEXP nthreads;
  PROTECT(nthreads = allocVector(INTSXP, 1));
  
  #if RTH_OMP
  if (rth_nthreads != NULL)
    INT(nthreads) = rth_nthreads[0] - '0';
  else if (omp_nthreads != NULL)
    INT(nthreads) = omp_nthreads[0] - '0';
  else
    INT(nthreads) = omp_get_max_threads(); // omp_get_max_threads
  #else
  INT(nthreads) = -1;
  #endif
  
  
  UNPROTECT(1);
  return nthreads;
}



SEXP Rth_tbb_auto_threads()
{
  char *rth_nthreads = getenv("RTH_NUM_THREADS");
  
  SEXP ret;
  PROTECT(ret = allocVector(INTSXP, 1));
  
  #if RTH_TBB
  if (rth_nthreads != NULL)
    INT(ret) = rth_nthreads[0] - '0';
  else
    INT(ret) = -1;
  #else
  INT(ret) = -1;
  #endif
  
  UNPROTECT(1);
  return ret;
}

}

