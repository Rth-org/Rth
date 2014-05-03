// Rth convolution routine; conforming to R convolve() with type = 'open'

// arguments:
//    x,y:  input vectors
//    nx,ny:  pointers to x,y lengths 
//    z:  output vector, length nx+ny-1
//extern "C" void rthconv(double *x, int *nx, double *y, int *ny, double *z);

#include <thrust/device_vector.h>
#include <thrust/sequence.h>
#include <stdlib.h>
#include <stdio.h>
#include <ostream>
#include <algorithm>

#include <Rcpp.h>

struct do1i  // name connotes doing one value of i in i,j loop
{
   thrust::device_vector<double>::iterator x;  
   thrust::device_vector<double>::iterator y;  
   thrust::device_vector<double>::iterator z;  
   int nx,ny,nz;
   double *xd,*yd,*zd;

   // set up variables for random permutation, needed for load balance:
   thrust::device_vector<int> rp;  
   int *rndperm;

   do1i(thrust::device_vector<double>::iterator _x,
        thrust::device_vector<double>::iterator _y,
        thrust::device_vector<double>::iterator _z,
        int _nx, int _ny):
      x(_x),y(_y),z(_z),nx(_nx),ny(_ny) { 
         xd = thrust::raw_pointer_cast(&x[0]);
         yd = thrust::raw_pointer_cast(&y[0]);
         zd = thrust::raw_pointer_cast(&z[0]);
         // set up random permutation; need it on device, and not in
         // this stack frame (volatile)
         thrust::host_vector<int> hseq(nx);
         thrust::sequence(hseq.begin(),hseq.end(),0);
         std::random_shuffle(hseq.begin(),hseq.end());
         rp.resize(nx);
         thrust::copy(hseq.begin(),hseq.end(),rp.begin());
         rndperm = thrust::raw_pointer_cast(&rp[0]);
      }
   __device__
   void operator()(const int i)
   {  int j;  // handle one value of i in i,j loop
      int rpi = rndperm[i];
      double xdi = xd[rpi];
      for (j = 0; j < ny; j++)
         zd[rpi+j] += xdi * yd[ny-j-1];
   }
};



RcppExport SEXP rthconv(SEXP x_, SEXP y_)
{  
   const int nx = LENGTH(x_);
   const int ny = LENGTH(y_);
   const int nz = nx+ny-1;
   
   Rcpp::NumericVector x(x_);
   Rcpp::NumericVector y(y_);
   Rcpp::NumericVector z(nz);
   
   thrust::device_vector<double> dx(x.begin(), x.end());
   thrust::device_vector<double> dy(y.begin(), y.end());
   thrust::device_vector<double> dz(nz);
   
   thrust::fill(dz.begin(), dz.end(), 0.);
   thrust::counting_iterator<int> seqb(0);
   thrust::counting_iterator<int> seqe = seqb + nx;
   
   thrust::for_each(seqb, seqe, do1i(dx.begin(), dy.begin(), dz.begin(), nx, ny));
   thrust::copy(dz.begin(), dz.end(), z.begin());
   
   return z;
}

