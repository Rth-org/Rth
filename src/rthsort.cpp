
// Rth interface to Thrust sort, Rcpp version

#include <thrust/device_vector.h>
#include <thrust/sort.h>

#include <Rcpp.h>
#include "backend.h"


RcppExport SEXP rthsort_double(SEXP a, SEXP nthreads)
{
  Rcpp::NumericVector xa(a);
  
  #if RTH_OMP
  omp_set_num_threads(INT(nthreads));
  #elif RTH_TBB
  tbb::task_scheduler_init init(INT(nthreads));
  #endif
  
  // set up device vector and copy xa to it
  thrust::device_vector<double> dx(xa.begin(), xa.end());
  
  // sort, then copy back to xa
  thrust::sort(dx.begin(), dx.end());
  thrust::copy(dx.begin(), dx.end(), xa.begin());
  
  return xa;
}

RcppExport SEXP rthsort_int(SEXP a, SEXP nthreads)
{
  Rcpp::IntegerVector xa(a);
  
  #if RTH_OMP
  omp_set_num_threads(INT(nthreads));
  #elif RTH_TBB
  tbb::task_scheduler_init init(INT(nthreads));
  #endif
  
  thrust::device_vector<int> dx(xa.begin(), xa.end());
  thrust::sort(dx.begin(), dx.end());
  thrust::copy(dx.begin(), dx.end(), xa.begin());
  
  return xa;
}

