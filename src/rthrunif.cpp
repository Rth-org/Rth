// Don't know how much I trust this, as I haven't run it through
// any rng stress tests yet

#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/random/linear_congruential_engine.h>
#include <thrust/random/normal_distribution.h>

#include <stdint.h>

#include "backend.h"

#include <Rcpp.h>

extern "C" {
#include "hash.h"
}

#ifdef GPU
#define flouble float
#else
#define flouble double
#endif


/* Random Uniform */
struct parallel_random_uniform : public thrust::unary_function<thrust::tuple<const unsigned int, const flouble, const flouble>, flouble>
{
  thrust::tuple<const unsigned int, const flouble, const flouble> t;
  parallel_random_uniform(thrust::tuple<const unsigned int, const flouble, const flouble> _t) : t(_t) {}

  __host__ __device__
  flouble operator()(unsigned int threadIdx)
  {
    unsigned int seed = thrust_hash(threadIdx) * thrust::get<0>(t);

    thrust::default_random_engine rng(seed);
    thrust::uniform_real_distribution<flouble> u01(thrust::get<1>(t), thrust::get<2>(t));

    return u01(rng);
  }
};



RcppExport SEXP rth_runif(SEXP n_, SEXP min_, SEXP max_, SEXP seed_, SEXP nthreads)
{
  int i;
  const uint64_t n = (uint64_t) REAL(n_)[0];
  const flouble min = (flouble) REAL(min_)[0];
  const flouble max = (flouble) REAL(max_)[0];
  const unsigned int seed = INTEGER(seed_)[0];
  
  #if RTH_OMP
  omp_set_num_threads(INT(nthreads));
  #elif RTH_TBB
  tbb::task_scheduler_init init(INT(nthreads));
  #endif
  
  thrust::device_vector<flouble> vec(n);
  
  thrust::tuple<const unsigned int, const flouble, const flouble> t(seed, min, max);
  
  thrust::transform(thrust::counting_iterator<int>(0),
    thrust::counting_iterator<int>(n),
    vec.begin(),
    parallel_random_uniform(t));
  
  thrust::host_vector<flouble> x(n);
  thrust::copy(vec.begin(), vec.end(), x.begin());
  
  return Rcpp::wrap(x);
}


