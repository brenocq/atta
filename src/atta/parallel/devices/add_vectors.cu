#include "add_vectors.h"
#include <cuda_runtime_api.h>
#include <stdio.h>

__global__ void add_vectors_kernel(float* a, float* b, float* c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}

void add_vectors(float* a, float* b, float* c, int n) {
    float* dev_a = 0;
    float* dev_b = 0;
    float* dev_c = 0;

    cudaMalloc((void**)&dev_a, n * sizeof(float));
    cudaMalloc((void**)&dev_b, n * sizeof(float));
    cudaMalloc((void**)&dev_c, n * sizeof(float));

    cudaMemcpy(dev_a, a, n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, n * sizeof(float), cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

    add_vectors_kernel<<<blocksPerGrid, threadsPerBlock>>>(dev_a, dev_b, dev_c, n);

    cudaMemcpy(c, dev_c, n * sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);
}
