#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/reduce.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>

#include "Rth.h"



/* mean */
struct div_by_n
{
  const int n;
  
  div_by_n(int _n) : n(_n) {}
  
  __host__ __device__
  flouble operator()(flouble x) const
  {
    return x/((flouble) n);
  }
};



// FIXME very slow
extern "C" SEXP rthmean(SEXP x, SEXP nthreads)
{
  SEXP avg;
  PROTECT(avg = allocVector(REALSXP, 1));
  const int n = LENGTH(x);
  
  RTH_GEN_NTHREADS(nthreads);
  
  thrust::device_vector<flouble> dx(REAL(x), REAL(x)+n);
  
  thrust::plus<flouble> binop;
  REAL(avg)[0] = (double) thrust::transform_reduce(dx.begin(), dx.end(), div_by_n(n), (flouble) 0., binop);
  
  UNPROTECT(1);
  return avg;
}

