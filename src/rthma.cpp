// Rth interface to Thrust moving-average example,
// simple_moving_average.cu in
// github.com/thrust/thrust/blob/master/examples/

// C++ code adapted from example in Thrust docs

#include <thrust/device_vector.h>
#include <thrust/scan.h>
#include <thrust/transform.h>
#include <thrust/functional.h>
#include <thrust/sequence.h>

#include "Rth.h"

// update function, to calculate current moving average value from the
// previous one
struct minus_and_divide : public thrust::binary_function<double,double,double>
{
  double w;
  minus_and_divide(double w) : w(w) {}
  __host__ __device__
  double operator()(const double& a, const double& b) const
  { return (a - b) / w; }
};



// computes moving averages from x of window width w
extern "C" SEXP rthma(SEXP x, SEXP w, SEXP nthreads)
{
  SEXP xb;
  const int xas = LENGTH(x);
  const int wa = INTEGER(w)[0];
  
  RTH_GEN_NTHREADS(nthreads);
  
  // set up device vector and copy xa to it
  thrust::device_vector<double> dx(REAL(x), REAL(x)+xas);
  if (xas < wa)
    return R_NilValue;
  
  // allocate device storage for cumulative sums, and compute them
  thrust::device_vector<double> csums(xas + 1);
  thrust::exclusive_scan(dx.begin(), dx.end(), csums.begin());
  // need one more sum at (actually past) the end
  csums[xas] = REAL(x)[xas-1] + csums[xas-1];
  
  // compute moving averages from cumulative sums
  PROTECT(xb = allocVector(REALSXP, xas - wa + 1));
  thrust::transform(csums.begin() + wa, csums.end(), 
    csums.begin(), REAL(xb), minus_and_divide(double(wa)));
  
  UNPROTECT(1);
  return xb;
}

