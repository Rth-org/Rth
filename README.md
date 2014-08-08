Rth
===

## What Is This?

Rth is a package of functions written in 
[Thrust](https://thrust.github.io/), 
callable from R. It thus provides to R programmers a set of parallel 
applications that run on both GPUs and multicore systems

Thrust is a C++ package for parallel processing. It was originally designed as 
a high-level approach to GPU programming, but actually can be compiled to 
multiple backends--GPUs (CUDA) and multicore (OpenMP, TBB). This ability to 
have the same parallel code run on both GPU and multicore systems is extremely 
valuable. (You need not have a GPU to run the multicore backends.) 

Operations are high-level, such as sorting, selecting, searching and prefix 
scan. From these basic building blocks, a myriad of parallel applications can 
be coded.

For example, consider Rth's `rthorder()`, which (in one of its options) serves as 
a parallel version of R's rank(). It first calls Thrust's `sort_by_key()` 
function, and then uses Thrust's `scatter()` function to permute the results in a 
way that produces the proper ranks.

R programmers can use Rth without knowing C++, Thrust, OpenMP or CUDA! But those 
who do know C++/Thrust can easily develop their own apps using the Rth approach.



## Pretty Good Parallelism

The Pretty Good Privacy package (PGP) gave the world encryption which, though 
not optimal, was simple to use and readily available. With Rth, the idea is to 
have PGP mean "pretty good parallelism"--not necessarily optimal, but easy to 
develop and with the key virtue that the same code gives reasonably good 
performance on both GPUs and multicore.



## Package Installation and Platform Requirements

To build the package, you will need to have the correct build environment set
up.  Exactly what that means depends on whether you wish to build Rth for use
with a gpu or multicore.

If you have a CUDA-capable NVIDIA GPU, you will need the nvcc compiler and CUDA 
development package.

For the multicore case, you will need a C++ compiler with an OpenMP library 
(this excludes clang++ at the time of writing).  GNU g++ should be sufficient.



## License and Copying

Copyright 2012-2014, Norm Matloff, with Contributions from:

* Drew Schmidt

The project is licensed under the GNU General Public License (GPL) 
version >= 2.
