#ifndef __RTH_HASH__
#define __RTH_HASH__

#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/random/linear_congruential_engine.h>
#include <thrust/random/normal_distribution.h>

unsigned int thrust_hash(unsigned int a);


#endif
