
// Rth histogram computation

// break data in to chunks, computing a histogram on each, then
// combining
   
#include <Rcpp.h>
#include <thrust/device_vector.h>

#include "backend.h"

#ifdef GPU
#define flouble float
#else
#define flouble double
#endif

typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<int>::iterator intveciter;
typedef thrust::device_vector<flouble> floublevec;
typedef thrust::device_vector<flouble>::iterator floubleveciter;
// typedef thrust::host_vector<int> hintvec;

struct bin1obs {  // find bin number for 1 data pt
   int n;
   flouble binwidth;
   flouble left;
   bin1obs(int _n,flouble _binwidth,flouble _left):
      n(_n),
      binwidth(_binwidth),
      left(_left)
      {}
   __device__
   int operator()(flouble xi)
   {  
      return (int) ((xi-left) / binwidth);
   }
};

struct do1chunk {  // handle one chunk of the data
   floubleveciter dx; intveciter dbincounts;
   int n,nbins,nch; 
   flouble left; flouble binwidth;
   flouble *wx; int *wb;
   do1chunk(floubleveciter _dx, intveciter _dbincounts,
      int _n, int _nbins, int _nch, 
      flouble _left, flouble _binwidth):
      dx(_dx), dbincounts(_dbincounts),
      n(_n),nbins(_nbins),nch(_nch),left(_left),binwidth(_binwidth)
      {  wx =  thrust::raw_pointer_cast(&dx[0]);  
         wb =  thrust::raw_pointer_cast(&dbincounts[0]);  
      }
   __device__
   void operator()(int chunknum)
   {  
      int chunksize = n / nch,
          first = chunknum * chunksize,
          last = first + chunksize - 1;
      if (chunknum == nch - 1) last = n - 1;
      int i, nb1 = nbins-1,binnum;
      for (i = first; i <= last; i++) {
         binnum = (wx[i]-left) / binwidth;
         if (binnum > nb1) binnum = nb1;
         wb[chunknum*nbins+binnum]++;
      }
   }
};



RcppExport SEXP rthhist(SEXP x_, SEXP nbins_,  SEXP nch_, SEXP nthreads)
{  
   Rcpp::NumericVector x(x_);
   const int n = x.size();
   const int nbins = INTEGER(nbins_)[0];
   const int nch = INTEGER(nch_)[0];
   floublevec dx(x.begin(), x.end());
   
   Rcpp::IntegerVector bincounts(nbins);
   Rcpp::NumericVector R_left(1);
   Rcpp::NumericVector R_binwidth(1);
   
   #if RTH_OMP
   omp_set_num_threads(INT(nthreads));
   #elif RTH_TBB
   tbb::task_scheduler_init init(INT(nthreads));
   #endif
   
   // determine binwidth etc.
   thrust::pair<floubleveciter, floubleveciter> mm = 
      thrust::minmax_element(dx.begin(), dx.end());
   flouble left = *(mm.first), right = *(mm.second);
   flouble binwidth = (right - left) / nbins;
   
   // form matrix of bin counts, one row per chunk
   intvec dbincounts(nch*nbins);
   
   
   // the heart of the computation, a for_each() loop, one iteration per
   // chunk
   thrust::counting_iterator<int> seqa(0);
   thrust::counting_iterator<int> seqb =  seqa + nch;
   thrust::for_each(seqa,seqb,
      do1chunk(dx.begin(),dbincounts.begin(
        
      ),n,nbins,nch,left,binwidth));
   
   // copy result to host and combine the subhistograms
   int hbincounts[nch*nbins];
   thrust::copy(dbincounts.begin(),dbincounts.end(),hbincounts);
   int binnum,chunknum;
   for (binnum = 0; binnum < nbins; binnum++) {
      int sum = 0;
      for (chunknum = 0; chunknum < nch; chunknum++) 
         sum += hbincounts[chunknum*nbins + binnum];
      bincounts[binnum] = sum;
   }
   
   
   REAL(R_left)[0] = (double) left; 
   REAL(R_binwidth)[0] = (double) binwidth;
   
   return Rcpp::List::create(Rcpp::Named("counts") = bincounts,
                             Rcpp::Named("left") = R_left,
                             Rcpp::Named("binwidth") = R_binwidth);
}

