//--------------------------------------------------
// Atta Utils Module
// cuda.h
// Date: 2023-05-102
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/cuda.h>
#include <cuda_runtime.h>

namespace atta::cuda {

#ifdef __NVCC__

#define CUDA_CHECK(ans) check((ans), __FILE__, __LINE__)
void check(cudaError_t code, const char* file, int line);

void* alloc(size_t size) {
    void* ptr = nullptr;
    CUDA_CHECK(cudaMalloc(&ptr, size));
    LOG_DEBUG("Cuda", "Cuda allocated $0", size);
    return ptr;
}

void free(void* ptr) { CUDA_CHECK(cudaFree(ptr)); }

void copyCpuToGpu(void* dst, void* src, size_t size) { CUDA_CHECK(cudaMemcpy(dst, src, size, cudaMemcpyHostToDevice)); }

void copyGpuToCpu(void* dst, void* src, size_t size) { CUDA_CHECK(cudaMemcpy(dst, src, size, cudaMemcpyDeviceToHost)); }

void check(cudaError_t code, const char* file, int line) {
    if (code != cudaSuccess)
        LOG_ERROR("atta::cuda", "CUDA error: [w]$0[] ($1:$2)", cudaGetErrorString(code), file, line);
}

#else

void* alloc(size_t size) { return nullptr; }

void free(void* ptr) {}

void copyCpuToGpu(void* dst, void* src, size_t size) {}

void copyGpuToCpu(void* dst, void* src, size_t size) {}

#endif

} // namespace atta::cuda
