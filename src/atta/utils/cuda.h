//--------------------------------------------------
// Atta Utils Module
// cuda.h
// Date: 2023-05-102
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_CUDA_H
#define ATTA_UTILS_CUDA_H

namespace atta::cuda {

uint8_t* alloc(size_t size);
void free(uint8_t* ptr);

void copyCpuToGpu(uint8_t* dst, uint8_t* src, size_t size);
void copyGpuToCpu(uint8_t* dst, uint8_t* src, size_t size);

} // namespace atta::cuda

#endif // ATTA_UTIL_CUDA_H