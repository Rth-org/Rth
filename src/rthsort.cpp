
// Rth interface to Thrust sort, Rcpp version

#include <thrust/device_vector.h>
#include <thrust/sort.h>

#include <Rcpp.h>

// the argument "a" will be replaced by its sorted version

RcppExport SEXP rthsort(SEXP a)
{  
   Rcpp::NumericVector xa(a);
   
   // set up device vector and copy xa to it
   thrust::device_vector<double> dx(xa.begin(), xa.end());
   
   // sort, then copy back to xa
   thrust::sort(dx.begin(), dx.end());
   thrust::copy(dx.begin(), dx.end(), xa.begin());
   
   return xa;  // copied back to original array
}

