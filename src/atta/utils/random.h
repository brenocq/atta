//--------------------------------------------------
// Atta Utils Module
// random.h
// Date: 2023-06-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_RANDOM_H
#define ATTA_UTILS_RANDOM_H
#include <atta/utils/common.h>

namespace atta::random {

ATTA_CPU_GPU void seed(uint64_t seed = 0);

ATTA_CPU_GPU float uniform(float min = 0.0f, float max = 1.0f);

ATTA_CPU_GPU float normal(float mean = 0.0f, float stdDev = 1.0f);

} // namespace atta::random

#endif // ATTA_UTILS_RANDOM_H
