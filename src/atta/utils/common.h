//--------------------------------------------------
// Atta Utils Module
// common.h
// Date: 2021-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_COMMON_H
#define ATTA_UTILS_COMMON_H

namespace atta {

#ifdef __NVCC__
#define ATTA_CPU_GPU __host__ __device__
#else
#define ATTA_CPU_GPU
#endif

} // namespace atta

#endif// ATTA_UTILS_COMMON_H
