// Don't know how much I trust this, as I haven't run it through
// any rng stress tests yet

#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/random/linear_congruential_engine.h>
#include <thrust/random/normal_distribution.h>

#include <stdint.h>

#include "Rth.h"

extern "C" {
#include "hash.h"
}



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
//    thrust::random::taus88 rng(seed);
    thrust::uniform_real_distribution<flouble> u01(thrust::get<1>(t), thrust::get<2>(t));

    return u01(rng);
  }
};



extern "C" SEXP rth_runif(SEXP n_, SEXP min_, SEXP max_, SEXP seed_, SEXP nthreads)
{
  SEXP x;
  const uint64_t n = (uint64_t) INTEGER(n_)[0];
  const flouble min = (flouble) REAL(min_)[0];
  const flouble max = (flouble) REAL(max_)[0];
  const unsigned int seed = INTEGER(seed_)[0];
  
  RTH_GEN_NTHREADS(nthreads);
  
  thrust::device_vector<flouble> vec(n);
  
  thrust::tuple<const unsigned int, const flouble, const flouble> t(seed, min, max);
  
  thrust::transform(thrust::counting_iterator<int>(0),
    thrust::counting_iterator<int>(n),
    vec.begin(),
    parallel_random_uniform(t));
  
//  thrust::host_vector<flouble> x(n);
  PROTECT(x = allocVector(REALSXP, n));
  thrust::copy(vec.begin(), vec.end(), REAL(x));
  
  return x;
}


