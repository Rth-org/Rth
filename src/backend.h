#ifndef __RTH_BACKEND__
#define __RTH_BACKEND__


#include <thrust/detail/config.h>

#define INT(x) INTEGER(x)[0]

// FIXME should we include CPP ?

#if THRUST_VERSION >= 100700
  #if THRUST_DEVICE_SYSTEM == THRUST_DEVICE_SYSTEM_OMP
  #include <omp.h>
  #define RTH_OMP 1

  #elif THRUST_DEVICE_SYSTEM == THRUST_DEVICE_SYSTEM_TBB
  #include "tbb/task_scheduler_init.h"
  #define RTH_TBB 1

  #elif THRUST_DEVICE_SYSTEM == THRUST_DEVICE_SYSTEM_CUDA
  #define RTH_CUDA 1
  #endif
#else
  #if THRUST_DEVICE_SYSTEM == THRUST_DEVICE_BACKEND_OMP
  #include <omp.h>
  #define RTH_OMP 1

  #elif THRUST_DEVICE_SYSTEM == THRUST_DEVICE_BACKEND_TBB
  #include "tbb/task_scheduler_init.h"
  #define RTH_TBB 1

  #elif THRUST_DEVICE_SYSTEM == THRUST_DEVICE_BACKEND_CUDA
  #define RTH_CUDA 1
  #endif
#endif

#endif
