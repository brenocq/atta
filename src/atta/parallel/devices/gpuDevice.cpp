//--------------------------------------------------
// Atta Parallel Module
// gpuDevice.cpp
// Date: 2023-03-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/devices/gpuDevice.h>
#include <atta/utils/cuda.h>
#include <cuda_runtime.h>

namespace atta::parallel {

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
            LOG_ERROR("pll::GpuDevice", "Unknown run capability version");
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
    LOG_DEBUG("pll::GpuDevice", "Found [w]$0[] GPUs with cuda support", deviceCount);

    // Select device
    for (int i = 0; i < deviceCount; i++) {
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, i);

        int numCoresPerSM = versionToCores(deviceProp.major, deviceProp.minor);
        LOG_INFO("pll::GpuDevice", "Device $0: [w]$1", i, deviceProp.name);
        LOG_INFO("pll::GpuDevice", " - Compute capability: [w]$0.$1", deviceProp.major, deviceProp.minor);
        LOG_INFO("pll::GpuDevice", " - Total global memory: [w]$0B", formatSize(deviceProp.totalGlobalMem));
        LOG_INFO("pll::GpuDevice", " - Total constant memory: [w]$0B", formatSize(deviceProp.totalConstMem));
        LOG_INFO("pll::GpuDevice", " - Total number of cores: [w]$0", deviceProp.multiProcessorCount * numCoresPerSM);
        LOG_INFO("pll::GpuDevice", " - Total shared memory per SM: [w]$0B", formatSize(deviceProp.sharedMemPerMultiprocessor));
        LOG_INFO("pll::GpuDevice", " - Number of SM: [w]$0", deviceProp.multiProcessorCount);
        LOG_INFO("pll::GpuDevice", " - Number of cores per SM: [w]$0", numCoresPerSM);
        LOG_INFO("pll::GpuDevice", " - Clock rate: [w]$0Hz", formatSize(deviceProp.clockRate * 1000, 1000));
    }

    LOG_SUCCESS("pll::GpuDevice", "Initialized!");
}

GpuDevice::~GpuDevice() {}

void GpuDevice::run(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) { 
    LOG_WARN("pll::GpuDevice", "std::function<void(uint32_t idx) run was not implemented for GPU yet");
}

// __global__ void kernel(scr::Script* script, cmp::EntityId firstClone, cmp::EntityId lastClone, float dt) {
//     cmp::EntityId clone = firstClone + (blockIdx.x * blockDim.x + threadIdx.x);
//     if (clone > lastClone)
//         return;
//     script->update(cmp::Entity(clone), dt);
// }

// void GpuDevice::run(scr::Script* script, cmp::Entity entity, float dt, uint32_t num) {
//     cmp::EntityId firstClone = entity.getId();
//     cmp::EntityId lastClone = firstClone + num;
//     kernel<<<(num + 255) / 256, 256>>>(script, firstClone, lastClone, dt);
// }

} // namespace atta::parallel
