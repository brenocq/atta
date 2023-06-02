//--------------------------------------------------
// Atta Processor Module
// gpu.cpp
// Date: 2023-03-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/processor/processors/gpu.h>
#include <atta/utils/cuda.h>
#include <cuda_runtime.h>

namespace atta::processor {

int versionToCores(int major, int minor) {
    // Returns the number of CUDA cores per multiprocessor for a given run capability
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
            LOG_ERROR("processor::Gpu", "Unknown run capability version");
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

Gpu::Gpu() : Processor(Type::GPU) {
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);
    LOG_DEBUG("processor::Gpu", "Found [w]$0[] GPUs with cuda support", deviceCount);

    // Select device
    for (int i = 0; i < deviceCount; i++) {
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, i);

        int numCoresPerSM = versionToCores(deviceProp.major, deviceProp.minor);
        LOG_INFO("processor::Gpu", "Device $0: [w]$1", i, deviceProp.name);
        LOG_INFO("processor::Gpu", " - Compute capability: [w]$0.$1", deviceProp.major, deviceProp.minor);
        LOG_INFO("processor::Gpu", " - Total global memory: [w]$0B", formatSize(deviceProp.totalGlobalMem));
        LOG_INFO("processor::Gpu", " - Total constant memory: [w]$0B", formatSize(deviceProp.totalConstMem));
        LOG_INFO("processor::Gpu", " - Total number of cores: [w]$0", deviceProp.multiProcessorCount * numCoresPerSM);
        LOG_INFO("processor::Gpu", " - Total shared memory per SM: [w]$0B", formatSize(deviceProp.sharedMemPerMultiprocessor));
        LOG_INFO("processor::Gpu", " - Number of SM: [w]$0", deviceProp.multiProcessorCount);
        LOG_INFO("processor::Gpu", " - Number of cores per SM: [w]$0", numCoresPerSM);
        LOG_INFO("processor::Gpu", " - Clock rate: [w]$0Hz", formatSize(deviceProp.clockRate * 1000, 1000));
    }

    LOG_SUCCESS("processor::Gpu", "Initialized!");
}

Gpu::~Gpu() {}

ATTA_GPU int d_count = 0;
ATTA_CONST int d_numSteps = 100000000;

__global__ void onStart() { d_count = 0; }

__global__ void step() {
    for (int i = 0; i < d_numSteps; i++) {
        d_count += 1;
    }
}

__global__ void onStop() { d_count = 1000; }

void printCount(std::string str = "") {
    int count;
    cudaMemcpyFromSymbol(&count, d_count, sizeof(int));
    LOG_DEBUG("GPU", "($1) Count $0", count, str);
}

void Gpu::startThread() { _thread = std::thread(&Gpu::loop, this); }

void Gpu::loop() {
    LOG_DEBUG("GPU", "Start kernel");
    printCount("Start");
    onStart<<<1, 1>>>();
    printCount("Step");
    while (shouldRun()) {
        step<<<1, 1>>>();
        printCount("Step");
    }
    onStop<<<1, 1>>>();
    printCount("Step");
}

} // namespace atta::processor
