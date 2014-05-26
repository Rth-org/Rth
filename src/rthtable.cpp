
// author: N. Matloff

// Rth implementation of R's table()

#include <thrust/device_vector.h>
#include <thrust/detail/config.h>

#include <Rcpp.h>


typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<int>::iterator intveciter;

// arguments:

// m:  data matrix m, in data frame form, i.e. with each column
//     representing a variable; coming from R, so assumed to be in
//     column-major order

// *nptr:  number of observations

// *nvptr:  number of variables

// dim:  variable i ranges over 1...dim[i]

// *ndimptr: product of all elts in dim

// freq:  one-dimensional vector of counts; index of variable 0 in m is
//        fastest-varying, that of variable nv-1 the slowest; e.g. with 3
//        variables, (0-based) index for (i,j,k) is k*rs + jr + i, where
//        r = dim[0] and s = dim[1]

// *nchptr:  number of chunks, i.e. number of threads to use

struct do1chunk {  // handle one chunk of the data
   intveciter dm,dfreq,ddim;
   int n,nv,nch,ndim;
   int *wm, *wf, *wd;
   do1chunk(intveciter _dm, intveciter _dfreq,
      int _n, int _nv, int _nch,
      intveciter _ddim, int _ndim):
      dm(_dm), dfreq(_dfreq), ddim(_ddim),
      n(_n),nv(_nv),nch(_nch),ndim(_ndim)
      {  wm =  thrust::raw_pointer_cast(&dm[0]);
         wf =  thrust::raw_pointer_cast(&dfreq[0]);
         wd =  thrust::raw_pointer_cast(&ddim[0]);
      }
   __device__
   void operator()(int chunknum)
   { 
      int chunksize = n / nch,
          first = chunknum * chunksize,
          last = first + chunksize - 1;
      if (chunknum == nch - 1) last = n - 1;
      int obsnum,varnum,count,prod;
      for (obsnum = first; obsnum <= last; obsnum++) {
         int base = obsnum;  // start of row i in wm
         count = 0; prod = 1;
         for (varnum = 0; varnum < nv; varnum++) {
            // adjust for data values starting at 1 not 0
            count += (wm[base+varnum*n]-1) * prod;
            prod *= wd[varnum];
         }
         wf[chunknum * ndim + count]++;
      }
   }
};



RcppExport SEXP rthtable(SEXP m_, SEXP n_, SEXP nv_, SEXP dim, SEXP ndim_, SEXP nch_)
{
  Rcpp::IntegerVector m(m_);
  const int n = INTEGER(n_)[0];
  const int nv = INTEGER(nv_)[0];
  const int lm = n * nv;
  const int ndim = INTEGER(ndim_)[0];
  int nch = INTEGER(nch_)[0];
  Rcpp::NumericVector freq(ndim);
  
  // if nch not specified, use Thrust to determine it
  if (nch == 0) {
    #ifdef THRUST_DEVICE_SYSTEM == THRUST_DEVICE_SYSTEM_TBB
    Rcpp::stop("you must specify nch>0 for TBB backend");
    #else
    thrust::system::detail::internal::uniform_decomposition<int>
      decomp1 =
      # if THRUST_DEVICE_SYSTEM == THRUST_DEVICE_SYSTEM_OMP
        thrust::system::omp::detail::default_decomposition(n);
      # else
        thrust::system::cuda::detail::default_decomposition(n);
      # endif
    nch =  decomp1.size();
    #endif
  }
  
  intvec dm(m.begin(),m.end());
  intvec ddim(INTEGER(dim), INTEGER(dim)+nv);
  intvec dfreq(nch*ndim);
  int hfreq[nch*ndim];
  thrust::counting_iterator<int> seqa(0);
  thrust::counting_iterator<int> seqb =  seqa + nch;
  thrust::for_each(seqa,seqb,
    do1chunk(dm.begin(),dfreq.begin(),n,nv,nch,ddim.begin(),ndim));
  thrust::copy(dfreq.begin(),dfreq.end(),hfreq);
  int binnum,chunknum;
  // sum up the bin frequencies across chunks
  for (binnum = 0; binnum < ndim; binnum++) {
    int sum = 0;
    for (chunknum = 0; chunknum < nch; chunknum++) 
      sum += hfreq[chunknum*ndim + binnum];
    freq[binnum] = sum;
  }
  
  return freq;
}


