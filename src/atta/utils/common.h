//--------------------------------------------------
// Atta Utils Module
// common.h
// Date: 2021-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_COMMON_H
#define ATTA_UTILS_COMMON_H

namespace atta {

#if __NVCC__
#define ATTA_GPU_CODE 1
#define ATTA_CPU_CODE 0
#else
#define ATTA_GPU_CODE 0
#define ATTA_CPU_CODE 1
#endif

#if ATTA_IS_GPU_CODE
#define ATTA_CPU __host__
#define ATTA_GPU __device__
#define ATTA_CPU_GPU __host__ __device__
#define ATTA_CONST __constant__
#else
#define ATTA_CPU
#define ATTA_GPU
#define ATTA_CPU_GPU
#define ATTA_CONST const
#endif

} // namespace atta

#endif // ATTA_UTILS_COMMON_H
