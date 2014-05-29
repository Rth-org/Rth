#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/reduce.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>

#include <Rcpp.h>

#ifdef GPU
#define flouble float
#else
#define flouble double
#endif


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
RcppExport SEXP rthmean(SEXP x_)
{
  Rcpp::NumericVector x(x_);
  Rcpp::NumericVector avg(1);
  const int n = LENGTH(x);
  
  thrust::device_vector<flouble> dx(x.begin(), x.end());
  
  thrust::plus<flouble> binop;
  flouble init = 0.;
  avg[0] = (double) thrust::transform_reduce(dx.begin(), dx.end(), div_by_n(n), init, binop);
  
  return avg;
}

