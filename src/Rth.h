#ifndef __RTH__
#define __RTH__


#ifdef GPU
#define flouble float
#else
#define flouble double
#endif

#include <R.h>
#include <Rinternals.h>

#include "backend.h"

#define RTH_ERROR -2147483648 // int NA in R

#define INT(x) INTEGER(x)[0]

#if RTH_OMP
  #define RTH_GEN_NTHREADS(nthreads) omp_set_num_threads(INT(nthreads))
#elif RTH_TBB
  #define RTH_GEN_NTHREADS(nthreads) tbb::task_scheduler_init init(INT(nthreads))
#else
  #define RTH_GEN_NTHREADS(nthreads) // nothing
#endif

#define INT(x) INTEGER(x)[0]

#endif
