#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/reduce.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>

#include "Rth.h"


typedef thrust::device_vector<flouble>::iterator floubleveciter;


/* gini coefficient */
struct compute_gini
{
  const int n;
  floubleveciter dx;
  flouble *x;
  
  compute_gini(int _n, floubleveciter _dx) : n(_n), dx(_dx) 
  {
    x =  thrust::raw_pointer_cast(&dx[0]);  
  }
  
  __host__ __device__
  flouble operator()(int ind) const
  {
    return ((flouble) (2*ind - n + 1))*x[ind];
  }
};


#define DBL(x) REAL(x)[0]

extern "C" SEXP rthgini(SEXP x, SEXP mu, SEXP unbiased_, SEXP nthreads)
{
  const int unbiased = INTEGER(unbiased_)[0];
  const int n = LENGTH(x);
  SEXP gini;
  PROTECT(gini = allocVector(REALSXP, 1));
  
  RTH_GEN_NTHREADS(nthreads);
  
  thrust::device_vector<double> dx(REAL(x), REAL(x)+n);
  
  thrust::sort(dx.begin(), dx.end());
  
  thrust::counting_iterator<int> begin(0);
  thrust::counting_iterator<int> end = begin + n;
  
  thrust::plus<flouble> binop;
  DBL(gini) = (double) thrust::transform_reduce(begin, end, compute_gini(n, dx.begin()), (flouble) 0., binop);
  
  if (unbiased)
    DBL(gini) = DBL(gini) / (n*(n-1)*DBL(mu));
  else
    DBL(gini) = DBL(gini) / (n*n*DBL(mu));
  
  return gini;
}


