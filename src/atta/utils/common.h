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
#define ATTA_IS_GPU_CODE 1
#else
#define ATTA_IS_GPU_CODE 0
#endif

#if ATTA_IS_GPU_CODE
#define ATTA_CPU_GPU __host__ __device__
#define ATTA_CONST __constant__
#else
#define ATTA_CPU_GPU
#define ATTA_CONST const
#endif

} // namespace atta

#endif// ATTA_UTILS_COMMON_H
