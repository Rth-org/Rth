// matrix transpose, from the Thrust package examples

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/gather.h>
#include <thrust/scan.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/iterator/transform_iterator.h>

#include<Rcpp.h>

// see comments at the top of rthxpos.cpp

// convert a linear index to a linear index in the transpose 
struct transpose_index : public thrust::unary_function<size_t,size_t>
{
  size_t nr, nc;

  __host__ __device__
  transpose_index(size_t _nr, size_t _nc) : nr(_nr), nc(_nc) {}

  __host__ __device__
  size_t operator()(size_t linear_index)
  {
      size_t i = linear_index % nr;
      size_t j = linear_index / nr;

      return nc * i + j;
  }
};

// convert a linear index to a row index
struct row_index : public thrust::unary_function<size_t,size_t>
{
  size_t n;

  __host__ __device__
  row_index(size_t _n) : n(_n) {}

  __host__ __device__
  size_t operator()(size_t i)
  {
      return i / n;
  }
};

// transpose an M-by-N array
template <typename T>
void transpose(size_t nr, size_t nc, thrust::device_vector<T>& src, thrust::device_vector<T>& dst)
{
  thrust::counting_iterator<size_t> indices(0);

  thrust::gather
    (thrust::make_transform_iterator(indices, transpose_index(nc, nr)),
     thrust::make_transform_iterator(indices, transpose_index(nc, nr)) + dst.size(),
     src.begin(),
     dst.begin());
}

RcppExport SEXP rthxposnv(SEXP m) 
{
   Rcpp::NumericMatrix tmpm = Rcpp::NumericMatrix(m);
   int nr = tmpm.nrow();
   int nc = tmpm.ncol();
   thrust::device_vector<double> dmat(tmpm.begin(),tmpm.end());
   // make space for the transpose
   thrust::device_vector<double> dxp(nr*nc);
   transpose(nr,nc,dmat,dxp);
   Rcpp::NumericVector routmat(nc*nr);
   thrust::copy(dxp.begin(),dxp.end(),routmat.begin());
   return routmat;
}

