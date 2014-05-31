
#include <stdio.h>
#include <math.h>
#include <thrust/device_vector.h>
#include <Rcpp.h>

#include "backend.h"

// Rth substitute for R's dist() function

// note that in addition to R using column-major order, Rcpp uses that
// format too when converting an R matrix to an Rcpp vector

// in output matrix, element in row i, column j gives the distance from
// row i in dm to row j of dm; output is a full matrix, not a
// triangular version

struct do1ival  
{
   const  thrust::device_vector<double>::iterator dm;  // input matrix 
   const  thrust::device_vector<double>::iterator dout;  // output matrix
   int nr,nc;
   double *m,*out;
   do1ival(thrust::device_vector<double>::iterator _dm, 
            thrust::device_vector<double>::iterator _dout, 
            int _nr, int _nc): 
      dm(_dm), dout(_dout), nr(_nr), nc(_nc)
      {
         m = thrust::raw_pointer_cast(&dm[0]);
         out = thrust::raw_pointer_cast(&dout[0]);
      }
   __device__
   void operator()(const int i)  
   // will compute and store all the distances from m's row i
   // to rows j of m, j > i 
   {  
      int j,k; double sum,tmp;
      if (i == nr - 1) return;
      for (j = i+1; j < nr; j++) {
         // find distance from row i to row j
         sum = 0;
         for (k = 0; k < nc; k++) {  // is column number
            tmp = m[k*nr+i] - m[k*nr+j];
            sum += tmp*tmp;
         }
         // result goes into elements (i,j) and (j,i) of out, a matrix 
         // of dimensions nr x nr
         tmp = sqrt(sum);
         out[j*nr+i] = out[i*nr+j] = tmp;
      }
   }
};

// compute distances from rows of inmat1 to rows of inmat2
RcppExport SEXP rthdist(SEXP inmat, SEXP nthreads)
{  
   Rcpp::NumericMatrix im(inmat);
   int nr = im.nrow();
   int nc = im.ncol();
   
   #if RTH_OMP
   omp_set_num_threads(INT(nthreads));
   #elif RTH_TBB
   tbb::task_scheduler_init init(INT(nthreads));
   #endif
   
   thrust::device_vector<double> dmat(im.begin(),im.end());
   // make space for the output
   thrust::device_vector<double> ddst(nr*nr);
   // iterators for row number of inmat
   thrust::counting_iterator<int> iseqb(0);
   thrust::counting_iterator<int> iseqe = iseqb + nr;
   // for each i in [iseqb,iseqe) find the distances from row i in inmat
   // to all rows j of inmat, j > i
   thrust::for_each(iseqb,iseqe,
      do1ival(dmat.begin(),ddst.begin(),nr,nc));
   Rcpp::NumericMatrix rout(nr,nr);
   thrust::copy(ddst.begin(),ddst.end(),rout.begin());
   return rout;
}

