#ifndef __RTH__
#define __RTH__


#include "backend.h"

#define RTH_GEN_NTHREADS(nthreads) \
  #if RTH_OMP \
  omp_set_num_threads(INT(nthreads)); \
  #elif RTH_TBB \
  tbb::task_scheduler_init init(INT(nthreads)); \
  #endif

#define INT(x) INTEGER(x)[0]

#endif
