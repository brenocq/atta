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

    void run(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) override;

    /**
     * @brief Run scripts with CUDA
     *
     * This function will execute scr::Script* for each entity using a CUDA kernel, the script will use and update the components that are in the GPU
     * global memory.
     *
     * @warn It is necessary to copy the components from the GPU to the CPU to see the changes in the UI.
     *
     * @param script Pointer to script (must have been allocated in the GPU)
     * @param entity First clone
     * @param dt Simulation dt
     * @param num Number of entities with sequential id to run
     */
    void run(scr::Script* script, cmp::Entity entity, float dt, uint32_t num) override;
};

} // namespace atta::parallel

#endif // ATTA_PARALLEL_DEVICES_GPU_DEVICE_H
