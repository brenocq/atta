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
};

} // namespace atta::parallel

#endif // ATTA_PARALLEL_DEVICES_GPU_DEVICE_H
