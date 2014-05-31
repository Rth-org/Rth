// author: N. Matloff

// Rth implementation of Kendall's tau

// assumes continuous variables, no ties of any kind

// could add randomization of i's for possibly better load balance, as
// in rthconv.cpp

// see also the method "kendall" in cor.fk(), part of the pcaPP package

#include <Rcpp.h>
#include <thrust/device_vector.h>

#include "backend.h"

#ifdef GPU
#define flouble float
#else
#define flouble double
#endif

typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<flouble> floublevec;

struct calcgti {  // handle 1 i, all j > i
   floublevec dx;
   floublevec dy;
   int n;
   flouble *wdx;
   flouble *wdy;
   calcgti(floublevec _dx,floublevec _dy,int _n) :
      dx(_dx),
      dy(_dy),
      n(_n)
      {  wdx = thrust::raw_pointer_cast(&dx[0]);   
         wdy = thrust::raw_pointer_cast(&dy[0]);  }
   __device__
   int operator()(int i)
   {  flouble xi = wdx[i], yi = wdy[i];
      int j,count=0;
      for (j = i+1; j < n; j++)
         count += ( (xi - wdx[j]) * (yi - wdy[j]) > 0);
      return count;
   }
};



RcppExport SEXP rthkendall(SEXP x_, SEXP y_, SEXP nthreads)
{
  Rcpp::NumericVector x(x_);
  Rcpp::NumericVector y(y_);
  Rcpp::NumericVector RET(1);
  const int n = LENGTH(x);
  
  #if RTH_OMP
  omp_set_num_threads(INT(nthreads));
  #elif RTH_TBB
  tbb::task_scheduler_init init(INT(nthreads));
  #endif
  
  thrust::counting_iterator<int> seqa(0);
  thrust::counting_iterator<int> seqb =  seqa + n - 1;
  floublevec dx(x.begin(), x.end());
  floublevec dy(y.begin(), y.end());
  
  intvec tmp(n-1);
  thrust::transform(seqa,seqb,tmp.begin(),calcgti(dx,dy,n));
  int totcount = thrust::reduce(tmp.begin(),tmp.end());
  flouble npairs = n * (n-1) / 2;
  
  REAL(RET)[0] = (double) (totcount - (npairs-totcount)) / npairs;
  
  return RET;
}


