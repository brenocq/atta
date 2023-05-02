//--------------------------------------------------
// Atta Utils Module
// cuda.h
// Date: 2023-05-102
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/cuda.h>
#include <cuda_runtime.h>

namespace atta::cuda {

#define CUDA_CHECK(ans) check((ans), __FILE__, __LINE__)
void check(cudaError_t code, const char* file, int line);

uint8_t* alloc(size_t size) {
    uint8_t* ptr = nullptr;
    CUDA_CHECK(cudaMalloc(&ptr, size));
    return ptr;
}

void free(uint8_t* ptr) { CUDA_CHECK(cudaFree(ptr)); }

void copyCpuToGpu(uint8_t* dst, uint8_t* src, size_t size) { CUDA_CHECK(cudaMemcpy(dst, src, size, cudaMemcpyHostToDevice)); }

void copyGpuToCpu(uint8_t* dst, uint8_t* src, size_t size) { CUDA_CHECK(cudaMemcpy(dst, src, size, cudaMemcpyDeviceToHost)); }

void check(cudaError_t code, const char* file, int line) {
    if (code != cudaSuccess)
        LOG_ERROR("atta::cuda", "CUDA error: [w]$0[] ($1:$2)", cudaGetErrorString(code), file, line);
}

} // namespace atta::cuda
