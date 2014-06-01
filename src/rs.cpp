
// non-Rcpp of Rth row sums; uses only R internal defines, e.g. REALSXP

// see comments at the top of rthxpos.cpp

#include <math.h>
#include <thrust/device_vector.h>

// note these includes

#include <R.h>
#include <Rdefines.h>

extern "C" {  // needed when code is C++

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

// return R vector of row sums of inmat
SEXP rs(SEXP inmat, SEXP nrow, SEXP ncol)
{
   int nr = INTEGER(nrow)[0];  // convert SEXP to C
   int nc = INTEGER(ncol)[0];
   double *inmat1 = REAL(inmat);
   // copy to device
   thrust::device_vector<double> dmat(inmat1,inmat1+nr*nc);
   // make space for the output
   thrust::device_vector<double> dsums(nr);
   // iterators for row number of inmat
   thrust::counting_iterator<int> iseqb(0);
   thrust::counting_iterator<int> iseqe = iseqb + nr;
   // for each i in [iseqb,iseqe) find sum of row i 
   thrust::for_each(iseqb,iseqe,
      do1ival(dmat.begin(),dsums.begin(),nr,nc));
   // prepare for R output
   SEXP rout;
   // shield from R garbage collection, not needed here but standard
   // anyway
   PROTECT(rout = allocVector(REALSXP,nr));
   thrust::copy(dsums.begin(),dsums.end(),REAL(rout));
   // remove the last 1 protected objects from the protection stack
   UNPROTECT(1);  
   return rout;
}

}
