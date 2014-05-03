
// author: N. Matloff

// Rth implementation of Kendall's tau

// assumes continuous variables, no ties of any kind

// could add randomization of i's for possibly better load balance, as
// in rthconv.cpp

// see also the method "kendall" in cor.fk(), part of the pcaPP package

#include <thrust/device_vector.h>

#ifdef GPU
#define flouble float
#else
#define flouble double
#endif

typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<flouble> floublevec;

struct calcgti {  // handle 1 i, all j > i
   floublevec dx;
   floublevec dy;
   int n;
   flouble *wdx;
   flouble *wdy;
   calcgti(floublevec _dx,floublevec _dy,int _n) :
      dx(_dx),
      dy(_dy),
      n(_n)
      {  wdx = thrust::raw_pointer_cast(&dx[0]);   
         wdy = thrust::raw_pointer_cast(&dy[0]);  }
   __device__
   int operator()(int i)
   {  flouble xi = wdx[i], yi = wdy[i];
      int j,count=0;
      for (j = i+1; j < n; j++)
         count += ( (xi - wdx[j]) * (yi - wdy[j]) > 0);
      return count;
   }
};

void rthkendall(flouble *x, flouble *y, int *nptr, flouble *tauptr)
{  int n = *nptr;
   thrust::counting_iterator<int> seqa(0);
   thrust::counting_iterator<int> seqb =  seqa + n - 1;
   floublevec dx(x,x+n);
   floublevec dy(y,y+n);
   intvec tmp(n-1);
   thrust::transform(seqa,seqb,tmp.begin(),calcgti(dx,dy,n));
   int totcount = thrust::reduce(tmp.begin(),tmp.end());
   flouble npairs = n * (n-1) / 2;
   *tauptr = (totcount - (npairs-totcount)) / npairs;
}

// #include <stdio.h>
// int main(int argc, char **argv)
// {
//    flouble x[6] = {3,5.01,12,6,5,15},
//            y[6] = {7,8,4.01,10,4,16};
//    int n = 6;
//    flouble tau;
//    rthkendall(x,y,&n,&tau);
//    printf("%f\n",tau);  // should be 0.46667
// }


