
// author: N. Matloff

// Rth replacements for R order(), rank()

#include <thrust/device_vector.h>
#include <thrust/sequence.h>
#include <thrust/sort.h>

#include <Rcpp.h>

#include "backend.h"

typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<double> doublevec;
typedef intvec::iterator intveciter;
typedef doublevec::iterator doubleveciter;

// returns order(x) in out, unless *rnk=TRUE, in which case 
// rank(x) is returned
//
// x is overwritten
RcppExport SEXP rthorder(SEXP x, SEXP rnk, SEXP nthreads) 
{  
   Rcpp::NumericVector rx(x);
   const int n = rx.size();
   bool rank = Rcpp::as<bool>(rnk);
   
   Rcpp::IntegerVector ret(n);
   
   #if RTH_OMP
   omp_set_num_threads(INT(nthreads));
   #elif RTH_TBB
   tbb::task_scheduler_init init(INT(nthreads));
   #endif
   
   doublevec dx(rx.begin(),rx.end());
   
   intvec seq(n);
   thrust::sequence(seq.begin(),seq.end());
   thrust::sort_by_key(dx.begin(),dx.end(),seq.begin());
   if (!rank) thrust::copy(seq.begin(),seq.end(),ret.begin());
   else {
      thrust::counting_iterator<int> seqa(0);
      thrust::counting_iterator<int> seqb =  seqa + n;
      intvec locout(n);
      thrust::scatter(seqa,seqb,seq.begin(),locout.begin()); 
      thrust::copy(locout.begin(),locout.end(),ret.begin());
   }
   
   return ret;
}



