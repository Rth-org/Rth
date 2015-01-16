#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/sort.h>

#include "Rth.h"


extern "C" SEXP rthsort_double(SEXP a, SEXP decreasing, SEXP inplace,
  SEXP nthreads)
{
  SEXP b;
  
  RTH_GEN_NTHREADS(nthreads);
  
  // set up device vector and copy xa to it
  thrust::device_vector<double> dx(REAL(a), REAL(a)+LENGTH(a));
  
  // sort, then copy back to R vector
  if (INTEGER(decreasing)[0])
    thrust::sort(dx.begin(), dx.end(), thrust::greater<double>());
  else
    thrust::sort(dx.begin(), dx.end());
  
  if (INTEGER(inplace)[0]) {
    thrust::copy(dx.begin(), dx.end(), REAL(a));
    return R_NilValue;
  }
  else
  {
    PROTECT(b = allocVector(REALSXP, LENGTH(a)));
    thrust::copy(dx.begin(), dx.end(), REAL(b));
    
    UNPROTECT(1);
    return b;
  }
}

extern "C" SEXP rthsort_int(SEXP a, SEXP decreasing, SEXP inplace,
   SEXP nthreads)
{
  SEXP b;
  
  #if RTH_OMP
  omp_set_num_threads(INT(nthreads));
  #elif RTH_TBB
  tbb::task_scheduler_init init(INT(nthreads));
  #endif
  
  thrust::device_vector<int> dx(INTEGER(a), INTEGER(a)+LENGTH(a));
  
  if (INTEGER(decreasing)[0])
    thrust::sort(dx.begin(), dx.end(), thrust::greater<int>());
  else
    thrust::sort(dx.begin(), dx.end());
  
  if (INTEGER(inplace)[0])
  {
    thrust::copy(dx.begin(), dx.end(), INTEGER(a));
    return R_NilValue;
  }
  else
  {
    PROTECT(b = allocVector(INTSXP, LENGTH(a)));
    thrust::copy(dx.begin(), dx.end(), INTEGER(b));
    
    UNPROTECT(1);
    return b;
  }
}

