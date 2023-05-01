//--------------------------------------------------
// Atta Parallel Module
// gpuDevice.h
// Date: 2023-03-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_DEVICES_GPU_DEVICE_H
#define ATTA_PARALLEL_DEVICES_GPU_DEVICE_H

#include <atta/parallel/devices/device.h>

namespace atta::parallel {

class GpuDevice : public Device {
  public:
    GpuDevice();
    ~GpuDevice();

    void compute(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) override;

  private:
    /**
     * @brief Copy components from CPU to GPU
     */
    void copyCpuToGpu();

    /**
     * @brief Copy components from GPU to CPU
     */
    void copyGpuToCpu();

    struct GpuComponentRegistry {
        uint32_t componentIndex; ///< Component index (same as cmp::ComponentRegistry)
        uint32_t componentSize;  ///< Component size in bytes
        uint8_t* memory;         ///< Start memory address for this component in the GPU
        uint32_t size;           ///< Number of component in the memory
    };

    /**
     * @brief GPU component registries
     *
     * Used to keep track of components in the global memory. The sequence of components stored in the GPU is the same as in the CPU.
     */
    std::vector<GpuComponentRegistry> _gpuComponentRegistries;

    /// GPU global memory used to store components
    uint8_t* _globalMem;
};

} // namespace atta::parallel

#endif // ATTA_PARALLEL_DEVICES_GPU_DEVICE_H
