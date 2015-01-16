// author: N. Matloff

// Rth replacements for R order(), rank()

#include <thrust/device_vector.h>
#include <thrust/sequence.h>
#include <thrust/sort.h>

#include "Rth.h"

typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<double> doublevec;
typedef intvec::iterator intveciter;
typedef doublevec::iterator doubleveciter;

// returns order(x) in out, unless *rnk=TRUE, in which case 
// rank(x) is returned
//
// x is overwritten
extern "C" SEXP rthorder(SEXP x, SEXP rnk, SEXP nthreads) 
{
  const int n = LENGTH(x);
  bool rank = (bool) INTEGER(rnk)[0];
  
  SEXP ret;
  PROTECT(ret = allocVector(INTSXP, n));
  
  RTH_GEN_NTHREADS(nthreads);
  
  doublevec dx(REAL(x), REAL(x)+n);
  
  intvec seq(n);
  thrust::sequence(seq.begin(),seq.end());
  thrust::sort_by_key(dx.begin(),dx.end(),seq.begin());
  if (!rank) 
    thrust::copy(seq.begin(), seq.end(), INTEGER(ret));
  else 
  {
    thrust::counting_iterator<int> seqa(0);
    thrust::counting_iterator<int> seqb =  seqa + n;
    intvec locout(n);
    thrust::scatter(seqa,seqb,seq.begin(),locout.begin()); 
    thrust::copy(locout.begin(), locout.end(), INTEGER(ret));
  }
  
  UNPROTECT(1);
  return ret;
}



