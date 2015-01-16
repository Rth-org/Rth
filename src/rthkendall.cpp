// author: N. Matloff

// Rth implementation of Kendall's tau

// assumes continuous variables, no ties of any kind

// could add randomization of i's for possibly better load balance, as
// in rthconv.cpp

// see also the method "kendall" in cor.fk(), part of the pcaPP package

#include <thrust/device_vector.h>

#include <R.h>
#include <Rinternals.h>

#include "backend.h"

#ifdef GPU
#define flouble float
#else
#define flouble double
#endif

typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<flouble> floublevec;

// handle 1 i, all j > i
struct calcgti {
  floublevec dx;
  floublevec dy;
  int n;
  flouble *wdx;
  flouble *wdy;
  calcgti(floublevec _dx,floublevec _dy,int _n) :
    dx(_dx),
    dy(_dy),
    n(_n)
    {
      wdx = thrust::raw_pointer_cast(&dx[0]);   
      wdy = thrust::raw_pointer_cast(&dy[0]);
    }
  __device__
  int operator()(int i)
  {
    flouble xi = wdx[i], yi = wdy[i];
    int j,count=0;
    for (j = i+1; j < n; j++)
      count += ( (xi - wdx[j]) * (yi - wdy[j]) > 0);
    return count;
  }
};



extern "C" SEXP rthkendall(SEXP x, SEXP y, SEXP nthreads)
{
  SEXP ret;
  PROTECT(ret = allocVector(REALSXP, 1));
  const int n = LENGTH(x);
  
  #if RTH_OMP
  omp_set_num_threads(INT(nthreads));
  #elif RTH_TBB
  tbb::task_scheduler_init init(INT(nthreads));
  #endif
  
  thrust::counting_iterator<int> seqa(0);
  thrust::counting_iterator<int> seqb =  seqa + n - 1;
  floublevec dx(REAL(x), REAL(x)+n);
  floublevec dy(REAL(y), REAL(y)+LENGTH(y)); // FIXME this makes me suspicious...
  
  intvec tmp(n-1);
  thrust::transform(seqa, seqb, tmp.begin(), calcgti(dx,dy,n));
  int totcount = thrust::reduce(tmp.begin(), tmp.end());
  flouble npairs = n * (n-1) / 2;
  
  REAL(ret)[0] = (double) (totcount - (npairs-totcount)) / npairs;
  
  return ret;
}


