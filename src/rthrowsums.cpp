
#include <math.h>
#include <thrust/device_vector.h>
#include <Rcpp.h>

#include<stdio.h>

// Rth substitute for R's rowSums()

// note that in addition to R using column-major order, Rcpp uses that
// format too when converting an R matrix to an Rcpp vector

// do one value of i in the for_each loop
struct do1ival  
{
   const  thrust::device_vector<double>::iterator dm;  // input matrix 
   const  thrust::device_vector<double>::iterator dout;  // output matrix
   int nr,nc;
   double *m,*out;
   do1ival(thrust::device_vector<double>::iterator _dm, 
            thrust::device_vector<double>::iterator _dout, 
            int _nr, int _nc): 
      dm(_dm),dout(_dout), nr(_nr), nc(_nc)
      {
         m = thrust::raw_pointer_cast(&dm[0]);
         out = thrust::raw_pointer_cast(&dout[0]);
      }
   __device__
   void operator()(const int i)  
   // will compute and store sum of m's row i
   {  int j;
      double sum = 0;
      for (j = 0; j < nc; j++) {
         sum += m[j*nr+i];
      }
      out[i] = sum;
   }
};

// return R vector of row usms of inmat
RcppExport SEXP rthrowsums(SEXP inmat)
{
   Rcpp::NumericMatrix im(inmat);
   int nr = im.nrow();
   int nc = im.ncol();
   // copy to device
   thrust::device_vector<double> dmat(im.begin(),im.end());
   // make space for the output
   thrust::device_vector<double> dsums(nr);
   // iterators for row number of inmat
   thrust::counting_iterator<int> iseqb(0);
   thrust::counting_iterator<int> iseqe = iseqb + nr;
   // for each i in [iseqb,iseqe) find sum of row i 
   thrust::for_each(iseqb,iseqe,
      do1ival(dmat.begin(),dsums.begin(),nr,nc));
   // prepare for R output
   Rcpp::NumericVector rout(nr);
   thrust::copy(dsums.begin(),dsums.end(),rout.begin());
   return rout;
}

