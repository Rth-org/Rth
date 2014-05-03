
// author: N. Matloff

// Rth implementation of R's table()

#include <thrust/device_vector.h>
#include <thrust/detail/config.h>



extern "C" {

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

// see parameter legend above
void rthtable(int *m, int *nptr, int *nvptr, int *dim, int *ndimptr,
// int *freq, int *nchptr)
   int *nchptr, int *freq)
{  int n = *nptr, nv = *nvptr, lm = n * nv, ndim = *ndimptr, nch = *nchptr;
   // if nch not specified, use Thrust to determine it
   if (nch == 0) {
      thrust::system::detail::internal::uniform_decomposition<int>
         decomp1 =
# if THRUST_DEVICE_SYSTEM == THRUST_DEVICE_SYSTEM_OMP
      thrust::system::omp::detail::default_decomposition(n);
# else
      thrust::system::cuda::detail::default_decomposition(n);
# endif
      nch =  decomp1.size();
   }
   intvec dm(m,m+lm);
   intvec ddim(dim,dim+nv);
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
}

// #include <stdio.h>
// int main(int argc, char **argv)
// {
//    int x[8] = {1,1,2,1,
//                2,1,3,2};
//    int dim[2] = {2,3};
//    int n = 4, nv = 2, ndim = 6, nch = 2,i;
//    int freq[100];
//    rthtable(x,&n,&nv,dim,&ndim,&nch,freq);
//    // should print 1 0 2 0 0 1
//    for (i = 0; i < ndim; i++) printf("%d\n",freq[i]);
//    printf("\n");
//    printf("\n");
//    int x2[15] = {1,1,2,2,1,
//                  2,1,3,3,2,
//                  2,1,1,2,2};
//    int dim2[3] = {2,3,2};
//    n = 5; nv = 3; ndim = 12, nch = 0;
//    rthtable(x2,&n,&nv,dim2,&ndim,&nch,freq);
//    // should print 1 0 0 0 0 1 0 0 2 0 0 1 
//    for (i = 0; i < ndim; i++) printf("%d\n",freq[i]);
// }

}

