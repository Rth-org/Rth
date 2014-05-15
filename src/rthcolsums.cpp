
// adapted from
// https://github.com/thrust/thrust/blob/master/examples/sum_rows.cu but

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/reduce.h>
#include <thrust/functional.h>
#include <thrust/random.h>
#include <iostream>

#include <Rcpp.h>

// convert col-major linear index to col index, for any 2-D array
// having nr rows; nr is stored in the functor
//
// convert a linear index to a column index
struct lintocol: public thrust::unary_function<int,int>
{
  int nr; // number of rows
  
  __host__ __device__
  lintocol(int nr) : nr(nr) {}

  __host__ __device__
  int operator()(int i)
  {
    return i / nr;
  }
};

// matrix m with nr rows 
RcppExport SEXP rthcolsums(SEXP m, SEXP nrow, SEXP ncol) {
   int nr = Rcpp::as<int>(nrow);
   int nc = Rcpp::as<int>(ncol);
   Rcpp::NumericVector xm = Rcpp::NumericVector(m);
   thrust::device_vector<double> colsums(nr);
   thrust::device_vector<int> colindices(nr);
   thrust::make_transform_iterator(thrust::counting_iterator<int>(0), 
      lintocol(nr)),
   // compute column sums by summing values with equal column indices
   thrust::reduce_by_key
      (
      // first argument to reduce_by_key() will be the beginning of the 
      // key vector; we start with the counting iterator, 0,1,2,..., and
      // apply our index-conversion functor to it
      thrust::make_transform_iterator(thrust::counting_iterator<int>(0), 
        lintocol(nr)),
      // second argument to reduce_by_key() will be the end of the
      // key vector, which is nr*nc elements past the beginning
      thrust::make_transform_iterator(thrust::counting_iterator<int>(0), 
         lintocol(nr)) + (nr*nc),
      xm.begin(),  // input matrix
      // this form of reduce_by_key has 2 outputs; the first is unique
      // key values (assumes keys are in sorted order), and the second is
      // the main output
      colindices.begin(),  
      colsums.begin(),
      // test consecutive keys being equal
      thrust::equal_to<int>(),
      // reduction uses summing
      thrust::plus<double>());
   Rcpp::NumericVector sums(nc);
   thrust::copy(colsums.begin(), colsums.end(),sums.begin());
   return sums;
}

