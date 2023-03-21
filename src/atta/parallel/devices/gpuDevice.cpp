//--------------------------------------------------
// Atta Parallel Module
// gpuDevice.cpp
// Date: 2023-03-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/devices/add_vectors.h>
#include <atta/parallel/devices/gpuDevice.h>
#include <cuda_runtime.h>

namespace atta::parallel {

int versionToCores(int major, int minor) {
    // Returns the number of CUDA cores per multiprocessor for a given compute capability
    // Based on table in NVIDIA CUDA Programming Guide
    switch (((major << 4) + minor)) {
        case 0x10:
            return 8;
        case 0x11:
            return 8;
        case 0x12:
            return 8;
        case 0x13:
            return 8;
        case 0x20:
            return 32;
        case 0x21:
            return 48;
        case 0x30:
            return 192;
        case 0x35:
            return 192;
        case 0x37:
            return 192;
        case 0x50:
            return 128;
        case 0x52:
            return 128;
        case 0x53:
            return 128;
        case 0x60:
            return 64;
        case 0x61:
            return 128;
        case 0x62:
            return 128;
        case 0x70:
            return 64;
        case 0x75:
            return 64;
        case 0x80:
            return 64;
        default:
            LOG_ERROR("parallel::GpuDevice", "Unknown compute capability version");
            return -1;
    }
}

std::string formatSize(unsigned long long number, int factor = 1024) {
    static const char* units[] = {"", "K", "M", "G", "T", "P", "E", "Z", "Y"};
    static const int numUnits = sizeof(units) / sizeof(units[0]);

    double size = number;
    int idx = 0;
    while (size >= factor && idx < numUnits - 1) {
        size /= factor;
        idx++;
    }

    char buf[32];
    sprintf(buf, "%.2f%s", size, units[idx]);
    return std::string(buf);
}

GpuDevice::GpuDevice() : Device(Type::GPU) {
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);
    LOG_DEBUG("parallel::GpuDevice", "Found [w]$0[] GPUs with cuda support", deviceCount);

    for (int i = 0; i < deviceCount; i++) {
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, i);

        int numCoresPerSM = versionToCores(deviceProp.major, deviceProp.minor);
        LOG_INFO("parallel::GpuDevice", "Device $0: [w]$1", i, deviceProp.name);
        LOG_INFO("parallel::GpuDevice", " - Compute capability: [w]$0.$1", deviceProp.major, deviceProp.minor);
        LOG_INFO("parallel::GpuDevice", " - Total global memory: [w]$0B", formatSize(deviceProp.totalGlobalMem));
        LOG_INFO("parallel::GpuDevice", " - Total constant memory: [w]$0B", formatSize(deviceProp.totalConstMem));
        LOG_INFO("parallel::GpuDevice", " - Total number of cores: [w]$0", deviceProp.multiProcessorCount * numCoresPerSM);
        LOG_INFO("parallel::GpuDevice", " - Total shared memory per SM: [w]$0B", formatSize(deviceProp.sharedMemPerMultiprocessor));
        LOG_INFO("parallel::GpuDevice", " - Number of SM: [w]$0", deviceProp.multiProcessorCount);
        LOG_INFO("parallel::GpuDevice", " - Number of cores per SM: [w]$0", numCoresPerSM);
        LOG_INFO("parallel::GpuDevice", " - Clock rate: [w]$0Hz", formatSize(deviceProp.clockRate * 1000, 1000));
    }

    const int n = 1024;
    float a[n], b[n], c[n];

    // Initialize the input vectors a and b
    for (int i = 0; i < n; ++i) {
        a[i] = static_cast<float>(i);
        b[i] = static_cast<float>(2 * i);
    }

    // Add the vectors a and b and store the result in c
    add_vectors(a, b, c, n);

    // Print the result vector c
    printf("%f\n", c[n - 1]);

    LOG_SUCCESS("parallel::GpuDevice", "Initialized!");
}

GpuDevice::~GpuDevice() {}

void GpuDevice::compute(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) {}

} // namespace atta::parallel
