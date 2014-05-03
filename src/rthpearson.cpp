
// author: N. Matloff

// Rth implementation of Pearson product-moment correlation

// single-pass, subject to increased roundoff error

#include <thrust/device_vector.h>
#include <thrust/inner_product.h>
#include <math.h>

#include <Rcpp.h>

typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<double> doublevec;

RcppExport SEXP rthpearson(SEXP x, SEXP y)
{  
   Rcpp::NumericVector xa(x);
   Rcpp::NumericVector ya(y);
   int n = xa.size();
   doublevec dx(xa.begin(),xa.end());
   doublevec dy(ya.begin(),ya.end());
   double zero = (double) 0.0;
   double xy =
      thrust::inner_product(dx.begin(),dx.end(),dy.begin(),zero);
   double x2 =
      thrust::inner_product(dx.begin(),dx.end(),dx.begin(),zero);
   double y2 =
      thrust::inner_product(dy.begin(),dy.end(),dy.begin(),zero);
   double xt =
      thrust::reduce(dx.begin(),dx.end());
   double yt =
      thrust::reduce(dy.begin(),dy.end());
   double xm = xt/n, ym = yt/n;
   double xsd = sqrt(x2/n - xm*xm);
   double ysd = sqrt(y2/n - ym*ym);
   double cor = (xy/n - xm*ym) / (xsd*ysd);
   return Rcpp::wrap(cor);
}

// #include <stdio.h>
// int main(int argc, char **argv)
// {
//    double x[6] = {3,5.01,12,6,5,15},
//            y[6] = {7,8,4.01,10,4,16};
//    int n = 6;
//    double rho;
//    rthpearson(x,y,&n,&rho);
//    printf("%f\n",rho);  // should be 0.46667
// }


