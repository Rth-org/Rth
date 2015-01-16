#include <stdio.h>
#include <math.h>
#include <thrust/device_vector.h>

#include <R.h>
#include <Rinternals.h>

#include "backend.h"

// Rth substitute for R's pdist package

// note that in addition to R using column-major order, Rcpp uses that
// format too when converting an R matrix to an Rcpp vector

// in output matrix, element in row i, column j gives the distance from
// row i in dm1 to row j of dm2

struct do1ival  
{
  const  thrust::device_vector<double>::iterator dm1;  // input matrix 
  const  thrust::device_vector<double>::iterator dm2;  // input matrix 
  const  thrust::device_vector<double>::iterator dout;  // output matrix
  int nr1,nc,nr2;
  double *m1,*m2,*out;
  do1ival(thrust::device_vector<double>::iterator _dm1, 
          thrust::device_vector<double>::iterator _dm2, 
          thrust::device_vector<double>::iterator _dout, 
          int _nr1, int _nc, int _nr2): 
    dm1(_dm1), dm2(_dm2),dout(_dout), nr1(_nr1), nc(_nc) , nr2(_nr2)
    {
      m1 = thrust::raw_pointer_cast(&dm1[0]);
      m2 = thrust::raw_pointer_cast(&dm2[0]);
      out = thrust::raw_pointer_cast(&dout[0]);
    }
  __device__
  void operator()(const int i)  
  // will compute and store all the distances from m1's row i
  // to rows of m2
  {
    int j,k; double sum,tmp;
    for (j = 0; j < nr2; j++) {
      // find distance from row i to row j
      sum = 0;
      for (k = 0; k < nc; k++) {  // is column number
        tmp = m1[k*nr1+i] - m2[k*nr2+j];
        sum += tmp*tmp;
      }
      // result goes into row i, column j of out, a matrix of
      // dimensions nr1 x nr2
      out[j*nr1+i] = sqrt(sum);
    }
  }
};

// compute distances from rows of inmat1 to rows of inmat2
extern "C" SEXP rthpdist(SEXP inmat1, SEXP inmat2, SEXP nthreads)
{
  SEXP rout;
  int nr1 = nrows(inmat1);
  int nc = ncols(inmat1);
  int nr2 = nrows(inmat2);
  
  #if RTH_OMP
  omp_set_num_threads(INT(nthreads));
  #elif RTH_TBB
  // tbb::task_scheduler_init init(INT(nthreads));
  // for unknown reasons, this code does not work under TBB
  #endif
  
  thrust::device_vector<double> dmat1(REAL(inmat1), REAL(inmat1)+nr1*nc);
  thrust::device_vector<double> dmat2(REAL(inmat2), REAL(inmat2)+nr2*nc);
  
  // make space for the output
  thrust::device_vector<double> ddst(nr1*nr2);
  
  // iterators for row number of inmat1
  thrust::counting_iterator<int> iseqb(0);
  thrust::counting_iterator<int> iseqe = iseqb + nr1;
  
  // for each i in [iseqb,iseqe) find the distances from row i in inmat1 
  // to all rows of inmat2
  thrust::for_each(iseqb,iseqe,
    do1ival(dmat1.begin(),dmat2.begin(),ddst.begin(),nr1,nc,nr2));
  
  PROTECT(rout = allocMatrix(REALSXP, nr1, nr2));
  thrust::copy(ddst.begin(), ddst.end(), REAL(rout));
  
  UNPROTECT(1);
  return rout;
}

