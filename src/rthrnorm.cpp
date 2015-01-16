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

#ifdef GPU
#define flouble float
#else
#define flouble double
#endif


/* Random Normal */
struct parallel_random_normal : public thrust::unary_function<thrust::tuple<const unsigned int, const flouble, const flouble>, flouble>
{
  thrust::tuple<const unsigned int, const flouble, const flouble> t;
  parallel_random_normal(thrust::tuple<const unsigned int, const flouble, const flouble> _t) : t(_t) {}
  
  __host__ __device__
  flouble operator()(unsigned int threadIdx)
  {
    unsigned int seed = thrust_hash(threadIdx) * thrust::get<0>(t);
    
    thrust::minstd_rand rng(seed);
    thrust::random::normal_distribution<flouble> nrm(thrust::get<1>(t), thrust::get<2>(t));
    
//    return nrm(rng);
    // FIXME shouldn't have to do this, but current normal_distribution is bugged...
    return thrust::get<1>(t) + thrust::get<2>(t)*nrm(rng);
  }
};



extern "C" SEXP rth_rnorm(SEXP n_, SEXP mean_, SEXP sd_, SEXP seed_, SEXP nthreads)
{
  SEXP x;
  const uint64_t n = (uint64_t) INTEGER(n_)[0];
  const flouble mean = (flouble) REAL(mean_)[0];
  const flouble sd = (flouble) REAL(sd_)[0];
  const unsigned int seed = INTEGER(seed_)[0];
  
  RTH_GEN_NTHREADS(nthreads);
  
  thrust::device_vector<flouble> vec(n);
  
  const thrust::tuple<const unsigned int, const flouble, const flouble> t(seed, mean, sd);
  
  thrust::transform(thrust::counting_iterator<int>(0),
    thrust::counting_iterator<int>(n),
    vec.begin(),
    parallel_random_normal(t));
  
//  thrust::host_vector<flouble> x(n);
  PROTECT(x = allocVector(REALSXP, n));
  thrust::copy(vec.begin(), vec.end(), REAL(x));
  
  UNPROTECT(1);
  return x;
}


