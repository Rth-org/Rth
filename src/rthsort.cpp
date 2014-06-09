
// Rth interface to Thrust sort, Rcpp version

#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/sort.h>

#include <Rcpp.h>
#include "backend.h"


RcppExport SEXP rthsort_double(SEXP a, SEXP decreasing, SEXP inplace,
   SEXP nthreads)
{
  Rcpp::NumericVector xa(a);
  
  #if RTH_OMP
  omp_set_num_threads(INT(nthreads));
  #elif RTH_TBB
  tbb::task_scheduler_init init(INT(nthreads));
  #endif
  
  // set up device vector and copy xa to it
  thrust::device_vector<double> dx(xa.begin(), xa.end());
  
  // sort, then copy back to R vector
  if (INTEGER(decreasing)[0])
    thrust::sort(dx.begin(), dx.end(), thrust::greater<double>());
  else
    thrust::sort(dx.begin(), dx.end());
  if (INTEGER(inplace)[0]) {
     thrust::copy(dx.begin(), dx.end(), xa.begin());
     // return xa;
  } else {
     Rcpp::NumericVector xb(xa.size());
     thrust::copy(dx.begin(), dx.end(), xb.begin());
     return xb;
  }
}

RcppExport SEXP rthsort_int(SEXP a, SEXP decreasing, SEXP inplace,
   SEXP nthreads)
{
  Rcpp::IntegerVector xa(a);
  
  #if RTH_OMP
  omp_set_num_threads(INT(nthreads));
  #elif RTH_TBB
  tbb::task_scheduler_init init(INT(nthreads));
  #endif
  
  thrust::device_vector<int> dx(xa.begin(), xa.end());
  
  if (INTEGER(decreasing)[0])
    thrust::sort(dx.begin(), dx.end(), thrust::greater<int>());
  else
    thrust::sort(dx.begin(), dx.end());
  
  if (INTEGER(inplace)[0]) {
     thrust::copy(dx.begin(), dx.end(), xa.begin());
     // return xa;
  } else {
     Rcpp::IntegerVector xb(xa.size());
     thrust::copy(dx.begin(), dx.end(), xb.begin());
     return xb;
  }
}

