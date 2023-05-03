//--------------------------------------------------
// Atta Parallel Module
// manager.h
// Date: 2023-02-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_MANAGER_H
#define ATTA_PARALLEL_MANAGER_H

#include <atta/parallel/devices/cpuDevice.h>
#include <atta/parallel/devices/device.h>
#include <atta/parallel/devices/gpuDevice.h>
#include <atta/parallel/devices/serialDevice.h>

namespace atta::parallel {

class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend void run(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func);
    friend void run(scr::Script* script, cmp::Entity entity, float dt, uint32_t num);
    friend void setDeviceType(Device::Type type);
    friend Device::Type getDeviceType();
    friend std::shared_ptr<Device> getDevice();
    friend std::shared_ptr<SerialDevice> getSerialDevice();
    friend std::shared_ptr<CpuDevice> getCpuDevice();
    friend std::shared_ptr<GpuDevice> getGpuDevice();

  private:
    void startUpImpl();
    void shutDownImpl();
    void runImpl(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func);
    void runImpl(scr::Script* script, cmp::Entity entity, float dt, uint32_t num);

    void setDeviceTypeImpl(Device::Type type);
    Device::Type getDeviceTypeImpl();

    std::shared_ptr<Device> getDeviceImpl();
    std::shared_ptr<SerialDevice> getSerialDeviceImpl();
    std::shared_ptr<CpuDevice> getCpuDeviceImpl();
    std::shared_ptr<GpuDevice> getGpuDeviceImpl();

    std::shared_ptr<Device> _device;             ///< Selected device
    std::shared_ptr<SerialDevice> _serialDevice; ///< Serial device
    std::shared_ptr<CpuDevice> _cpuDevice;       ///< CPU device
    std::shared_ptr<GpuDevice> _gpuDevice;       ///< GPU device
};

} // namespace atta::parallel

#endif // ATTA_PARALLEL_MANAGER_H
