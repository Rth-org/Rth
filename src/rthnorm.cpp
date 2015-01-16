#include <thrust/functional.h>
#include <thrust/transform.h>
#include <thrust/transform_reduce.h>
#include <thrust/device_vector.h>

#include <cmath>
#include <R.h>
#include <Rinternals.h>

#include "backend.h"

#ifdef GPU
#define flouble float
#else
#define flouble double
#endif

template <typename T>
struct ppow
{
  double p;
  
  __host__ __device__
  T operator()(const T& x) const 
  { 
    return std::pow(std::abs(x), p);
  }
};

template <typename T>
struct ppow1
{
  __host__ __device__
  T operator()(const T& x) const 
  { 
    return std::abs(x);
  }
};

template <typename T>
struct ppow2
{
  __host__ __device__
  T operator()(const T& x) const 
  { 
    return x*x;
  }
};



// Norm of vector
double calc_norm(double *x, const int len, const double p)
{
  thrust::plus<double> binary_op;
  double norm;
  
  thrust::device_vector<double> dx(x, x + len);
  
  
  if (p == 2) // Frobenius aka 2-norm
  {
    norm = thrust::transform_reduce(dx.begin(), dx.end(), ppow2<double>(), 0., binary_op);
    norm = std::sqrt(norm);
  }
  else if (p == 1) // 1-norm
    norm = thrust::transform_reduce(dx.begin(), dx.end(), ppow1<double>(), 0., binary_op);
  else
  {
    norm = thrust::transform_reduce(dx.begin(), dx.end(), ppow<double>(), 0., binary_op);
    norm = std::pow(norm, 1./p);
  }
  return norm;
}



// Euclidean distance of two vectors
double calc_dist(double *x, const double *y, const int len, const double p)
{
  int i;
  double *diff;
  double dist;
  
  diff = (double *) malloc(len * sizeof(diff));
  
  for (i=0; i<len; i++)
    diff[i] = x[i] - y[i];
  
  dist = calc_norm(diff, len, p);
  
  free(diff);
  
  return dist;
}



// Wrappers
extern "C" SEXP rth_norm(SEXP x_, SEXP p_, SEXP nthreads)
{
  double *x = REAL(x_);
  int len = LENGTH(x_);
  double p = REAL(p_)[0];
  
  SEXP nrm;
  PROTECT(nrm = allocVector(REALSXP, 1));
  
  #if RTH_OMP
  omp_set_num_threads(INT(nthreads));
  #elif RTH_TBB
  tbb::task_scheduler_init init(INT(nthreads));
  #endif
  
  REAL(nrm)[0] = calc_norm(x, len, p);
  
  UNPROTECT(1);
  return nrm;
}



extern "C" SEXP rth_dist(SEXP x_, SEXP y_, SEXP p_)
{
  double *x = REAL(x_);
  double *y = REAL(y_);
  const double p = REAL(p_)[0];
  const int len = LENGTH(x_);
  
  SEXP dist;
  PROTECT(dist = allocVector(REALSXP, 1));
  
  REAL(dist)[0] = calc_dist(x, y, len, p);
  
  UNPROTECT(1);
  return dist;
}

