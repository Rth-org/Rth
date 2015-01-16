// author: N. Matloff

// Rth implementation of Pearson product-moment correlation

// single-pass, subject to increased roundoff error

#include <thrust/device_vector.h>
#include <thrust/inner_product.h>
#include <math.h>

#include "Rth.h"

typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<double> doublevec;

extern "C" SEXP rthpearson(SEXP x, SEXP y, SEXP nthreads)
{
  SEXP cor;
  int n = LENGTH(x);
  doublevec dx(REAL(x), REAL(x)+n);
  doublevec dy(REAL(x), REAL(x)+n);
  double zero = (double) 0.0;
  
  RTH_GEN_NTHREADS(nthreads);
  
  double xy =
    thrust::inner_product(dx.begin(), dx.end(), dy.begin(), zero);
  double x2 =
    thrust::inner_product(dx.begin(), dx.end(), dx.begin(), zero);
  double y2 =
    thrust::inner_product(dy.begin(), dy.end(), dy.begin(), zero);
  double xt =
    thrust::reduce(dx.begin(), dx.end());
  double yt =
    thrust::reduce(dy.begin(), dy.end());
  double xm = xt/n, ym = yt/n;
  double xsd = sqrt(x2/n - xm*xm);
  double ysd = sqrt(y2/n - ym*ym);
  
  PROTECT(cor = allocVector(REALSXP, 1));
  REAL(cor)[0] = (xy/n - xm*ym) / (xsd*ysd);
  
  UNPROTECT(1);
  return cor;
}


