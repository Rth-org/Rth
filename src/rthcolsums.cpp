
// adapted from
// https://github.com/thrust/thrust/blob/master/examples/sum_rows.cu but

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/reduce.h>
#include <thrust/functional.h>

#include <Rcpp.h>

// note that R uses column-major order, and Rcpp vectors retain this
// ordering

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
RcppExport SEXP rthcolsums(SEXP m) {
   Rcpp::NumericMatrix tmpm = Rcpp::NumericMatrix(m);
   int nr = tmpm.nrow();
   int nc = tmpm.ncol();
   Rcpp::NumericVector xm = Rcpp::NumericVector(m);
   // colsums will be device vector for the column sums
   thrust::device_vector<double> colsums(nr);
   //
   // reduce_by_key(), in the form here, takes input keys, in this 
   // case column numbers, and then does reductions based on their
   // unique values; data that have the same input keys will be 
   // reduced as a group (it's assumed that the data ordering is such 
   // that all data with the same input keys are contiguous)
   //
   // colindices will be the "output keys"
   thrust::device_vector<int> colindices(nr);
   thrust::reduce_by_key
      (
      // start of input key vector; we take the counting iterator, 
      // 0,1,2,..., and apply our index-conversion functor to it
      thrust::make_transform_iterator(thrust::counting_iterator<int>(0), 
        lintocol(nr)),
      // end of the input key vector, 
      thrust::make_transform_iterator(thrust::counting_iterator<int>(0), 
         lintocol(nr)) + (nr*nc),
      xm.begin(),  // data, in this case our input matrix
      // output keys, not used in this case, but deals with "empty rows"
      // in "ragged matrix" settings
      colindices.begin(),  
      // at long last, our column sums
      colsums.begin());
   // prepare to convert to R form
   Rcpp::NumericVector rrowsums(nc);
   thrust::copy(colsums.begin(), colsums.end(),rrowsums.begin());
   return rrowsums;
}

