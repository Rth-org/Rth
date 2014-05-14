// Don't know how much I trust this, as I haven't run it through
// any rng stress tests yet

#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/random/linear_congruential_engine.h>
#include <thrust/random/normal_distribution.h>

#include <Rcpp.h>

#ifdef GPU
#define flouble float
#else
#define flouble double
#endif


// The hash function from the thrust devs
__host__ __device__
unsigned int thrust_hash(unsigned int a)
{
  a = (a+0x7ed55d16) + (a<<12);
  a = (a^0xc761c23c) ^ (a>>19);
  a = (a+0x165667b1) + (a<<5);
  a = (a+0xd3a2646c) ^ (a<<9);
  a = (a+0xfd7046c5) + (a<<3);
  a = (a^0xb55a4f09) ^ (a>>16);
  return a;
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
    thrust::uniform_real_distribution<flouble> u01(thrust::get<1>(t), thrust::get<2>(t));

    return u01(rng);
  }
};



RcppExport SEXP rth_runif(SEXP n_, SEXP min_, SEXP max_, SEXP seed_)
{
  int i;
  const int n = INTEGER(n_)[0];
  const flouble min = (flouble) REAL(min_)[0];
  const flouble max = (flouble) REAL(max_)[0];
  const unsigned int seed = INTEGER(seed_)[0];


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

    return nrm(rng);
  }
};



RcppExport SEXP rth_rnorm(SEXP n_, SEXP mean_, SEXP sd_, SEXP seed_)
{
  int i;
  const int n = INTEGER(n_)[0];
  const flouble mean = (flouble) REAL(mean_)[0];
  const flouble sd = (flouble) REAL(sd_)[0];
  const unsigned int seed = INTEGER(seed_)[0];


  thrust::device_vector<flouble> vec(n);

  thrust::tuple<const unsigned int, const flouble, const flouble> t(seed, mean, sd);

  thrust::transform(thrust::counting_iterator<int>(0),
    thrust::counting_iterator<int>(n),
    vec.begin(),
    parallel_random_normal(t));

  thrust::host_vector<flouble> x(n);
  thrust::copy(vec.begin(), vec.end(), x.begin());

  return Rcpp::wrap(x);
}


