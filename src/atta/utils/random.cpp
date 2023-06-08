//--------------------------------------------------
// Atta Utils Module
// random.cpp
// Date: 2023-06-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/random.h>
#include <random>
#ifdef __NVCC__
#include <curand_kernel.h>
#endif

namespace atta::random {

// TODO Random number generated are not thread safe
#ifdef __NVCC__
ATTA_GPU curandState _state;
#endif
ATTA_CPU std::mt19937 _gen;

//---------- CPU ----------//
ATTA_CPU void seedCPU(uint64_t seed) {
    if (seed == 0) {
        // Random seed
        std::random_device rd;
        auto s = rd();
        _gen = std::mt19937(s);
    } else {
        // Fixed seed
        _gen = std::mt19937(seed);
    }
}

ATTA_CPU float uniformCPU(float min, float max) {
    std::uniform_real_distribution<float> dis(min, max);
    return dis(_gen);
}

ATTA_CPU float normalCPU(float mean, float stdDev) {
    std::normal_distribution<float> dis(mean, stdDev);
    return dis(_gen);
}

//---------- GPU ----------//
ATTA_GPU void seedGPU(uint64_t seed) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (seed == 0) {
        curand_init(clock64(), tid, 0, &_state);
    } else {
        curand_init(seed, tid, 0, &_state);
    }
}

ATTA_GPU float uniformGPU(float min, float max) { return curand_uniform(&_state) * (max - min) + min; }

ATTA_GPU float normalGPU(float mean, float stdDev) { return curand_normal(&_state) * stdDev + mean; }

//---------- CPU/GPU ----------//
ATTA_CPU_GPU void seed(uint64_t seed) {
#if ATTA_CPU_CODE
    seedCPU(seed);
#else
    seedGPU(seed);
#endif
}

ATTA_CPU_GPU float uniform(float min, float max) {
#if ATTA_CPU_CODE
    return uniformCPU(min, max);
#else
    return uniformGPU(min, max);
#endif
}

ATTA_CPU_GPU float normal(float mean, float stdDev) {
#if ATTA_CPU_CODE
    return normalCPU(mean, stdDev);
#else
    return normalGPU(mean, stdDev);
#endif
}

} // namespace atta::random
