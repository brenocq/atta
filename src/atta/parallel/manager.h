//--------------------------------------------------
// Atta Parallel Module
// manager.h
// Date: 2023-02-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_MANAGER_H
#define ATTA_PARALLEL_MANAGER_H

#include <atta/parallel/devices/device.h>
#include <atta/parallel/devices/cpuDevice.h>

namespace atta::parallel {

class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend void parallelFor(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func);
    friend void setDevice(Device::Type type);
    friend Device::Type getDevice();

  private:
    void startUpImpl();
    void shutDownImpl();
    void parallelForImpl(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func);

    void setDeviceImpl(Device::Type type);
    Device::Type getDeviceImpl();

    std::shared_ptr<Device> _device;///< Selected device
    std::shared_ptr<CpuDevice> _cpuDevice;///< CPU device
};

} // namespace atta::parallel

#endif // ATTA_PARALLEL_MANAGER_H
