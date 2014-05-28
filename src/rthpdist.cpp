
#include <stdio.h>
#include <math.h>
#include <thrust/device_vector.h>
#include <Rcpp.h>

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
RcppExport SEXP rthpdist(SEXP inmat1,SEXP inmat2)
{  
   Rcpp::NumericMatrix im1(inmat1);
   Rcpp::NumericMatrix im2(inmat2);
   int nr1 = im1.nrow();
   int nc = im1.ncol();
   int nr2 = im2.nrow();
   thrust::device_vector<double> dmat1(im1.begin(),im1.end());
   thrust::device_vector<double> dmat2(im2.begin(),im2.end());
   // make space for the output
   thrust::device_vector<double> ddst(nr1*nr2);
   // iterators for row number of inmat1
   thrust::counting_iterator<int> iseqb(0);
   thrust::counting_iterator<int> iseqe = iseqb + nr1;
   // for each i in [iseqb,iseqe) find the distances from row i in inmat1 
   // to all rows of inmat2
   thrust::for_each(iseqb,iseqe,
      do1ival(dmat1.begin(),dmat2.begin(),ddst.begin(),nr1,nc,nr2));
   Rcpp::NumericMatrix rout(nr1,nr2);
   thrust::copy(ddst.begin(),ddst.end(),rout.begin());
   return rout;
}

// int rand16()  // generate random integers mod 16
// {  return rand() % 16;  }
// 
// // print m, row-by-row, but assuming column-major storage
// void printm(double *m, int nr, int nc) {
//    int i,j;
//    for (i = 0; i < nr; i++) {
//       for (j = 0; j < nc; j++) 
//          printf("%f  ",m[j*nr+i]);
//       printf("\n");
//    }
// }
// 
// // test code:  cmd line args are matrix size, then row, col of elt to be
// // checked
// 
// int main(int argc, char **argv)
// {  int nr = atoi(argv[1]); int nc = nr;
//    // int i,j;
//    double *mat1 = (double *) malloc(nr*nc*sizeof(double));
//    double *mat2 = (double *) malloc(nr*nc*sizeof(double));
//    double *dmat = (double *) malloc(nr*nr*sizeof(double));
//    thrust::generate(mat1,mat1+nr*nc,rand16);
//    thrust::generate(mat2,mat2+nr*nc,rand16);
//    rthpdist(mat1,mat2,dmat,nr,nc,nr);
//    if (nr < 100)
//    printm(mat1,nr,nc);
//    if (nr < 100)
//    printm(mat2,nr,nc);
//    if (nr < 100)
//    printm(dmat,nr,nr);
// }

