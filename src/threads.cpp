#include <R.h>
#include <Rinternals.h>

#include "backend.h"


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


SEXP Rth_omp_set_num_threads(SEXP n)
{
  #if RTH_OMP
  omp_set_num_threads(INT(n));
  #endif
  
  return R_NilValue;
}



SEXP Rth_omp_get_num_threads()
{
  SEXP nthreads;
  PROTECT(nthreads = allocVector(INTSXP, 1));
  
  #if RTH_OMP
  INT(nthreads) = omp_get_num_threads(); // omp_get_max_threads
  #else
  INT(nthreads) = -1;
  #endif
  
  // FIXME add tbb::task_scheduler_init::default_num_threads() for TBB ???
  
  UNPROTECT(1);
  return nthreads;
}



SEXP Rth_tbb_auto_threads()
{
  SEXP ret;
  PROTECT(ret = allocVector(INTSXP, 1));
  
  #if RTH_TBB
  INT(ret) = tbb::task_scheduler_init::automatic;
  #else
  INT(ret) = -1;
  #endif
  
  UNPROTECT(1);
  return ret;
}

}

