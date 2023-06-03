//--------------------------------------------------
// Atta Utils Module
// cuda.h
// Date: 2023-05-102
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_CUDA_H
#define ATTA_UTILS_CUDA_H

namespace atta::cuda {

void* alloc(size_t size);
void free(void* ptr);

void copyCpuToGpu(void* dst, void* src, size_t size);
void copyGpuToCpu(void* dst, void* src, size_t size);

} // namespace atta::cuda

#endif // ATTA_UTIL_CUDA_H
