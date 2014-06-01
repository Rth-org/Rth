
// matrix transpose, for_each version

// see the alternate version, rthxposnv.cpp that follows the example in
// the Thrust package; the latter is an instructive example of the use
// of thrust::scatter()

#include <thrust/device_vector.h>

#include<Rcpp.h>

// functor; holds iterators for the input and output matrices, and each
// invocation of the function copies from one element from the former to
// the latter
struct copyelt2xp  
{
   int nrow;  
   int ncol;  
   const  thrust::device_vector<double>::iterator m;  // input matrix 
   const  thrust::device_vector<double>::iterator mxp;  // output matrix
   double *m1,*mxp1;
   copyelt2xp(thrust::device_vector<double>::iterator _m, 
            thrust::device_vector<double>::iterator _mxp, 
            int _nr, int _nc): 
      m(_m), mxp(_mxp), nrow(_nr), ncol(_nc) {
         m1 = thrust::raw_pointer_cast(&m[0]);
         mxp1 = thrust::raw_pointer_cast(&mxp[0]);
      }
   __device__
   void operator()(const int i)  
   // copies the i-th element of the input matrix to the output matrix
   {  // elt i in input is row r, col c there
      int c = i / nrow; int r = i % nrow;  
      // that elt will be row c and col r in output, which has nrow
      // cols, so copy as follows
      mxp1[r*ncol+c] = m1[c*nrow+r];
   }
};

RcppExport SEXP rthxpos(SEXP m) 
{
   Rcpp::NumericMatrix tmpm = Rcpp::NumericMatrix(m);
   int nr = tmpm.nrow();
   int nc = tmpm.ncol();
   thrust::device_vector<double> dmat(tmpm.begin(),tmpm.end());
   // make space for the transpose
   thrust::device_vector<double> dxp(nr*nc);
   // iterator to march through the matrix elements
   thrust::counting_iterator<int> seqb(0);
   thrust::counting_iterator<int> seqe = seqb + nr*nc;
   // for each i in seq, copy the matrix elt to its spot in the
   // transpose
   thrust::for_each(seqb,seqe,
      copyelt2xp(dmat.begin(),dxp.begin(),nr,nc));
   // prepare the R output, and return it
   Rcpp::NumericVector routmat(nc*nr);
   thrust::copy(dxp.begin(),dxp.end(),routmat.begin());
   return routmat;
}

