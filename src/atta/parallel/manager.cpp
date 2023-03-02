//--------------------------------------------------
// Atta Parallel Module
// manager.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/manager.h>

namespace atta::parallel {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    _serialDevice = std::make_shared<SerialDevice>();
    _cpuDevice = std::make_shared<CpuDevice>();
    _gpuDevice = std::make_shared<GpuDevice>();
    _device = _serialDevice;
}

void Manager::shutDownImpl() {
    _device.reset();
    _serialDevice.reset();
    _cpuDevice.reset();
}

void Manager::computeImpl(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) { _device->compute(start, end, std::move(func)); }

void Manager::setDeviceTypeImpl(Device::Type type) {
    switch (type) {
        case Device::Type::SERIAL:
            _device = _serialDevice;
            LOG_SUCCESS("parallel::Manager", "Device [w]SERIAL[] selected");
            break;
        case Device::Type::CPU:
            _device = _cpuDevice;
            LOG_SUCCESS("parallel::Manager", "Device [w]CPU[] selected");
            break;
        default:
            LOG_WARN("parallel::Manager", "This device is not supported yet");
    }
}

Device::Type Manager::getDeviceTypeImpl() { return _device->getType(); }
std::shared_ptr<Device> Manager::getDeviceImpl() { return _device; }
std::shared_ptr<SerialDevice> Manager::getSerialDeviceImpl() { return _serialDevice; }
std::shared_ptr<CpuDevice> Manager::getCpuDeviceImpl() { return _cpuDevice; }
std::shared_ptr<GpuDevice> Manager::getGpuDeviceImpl() { return _gpuDevice; }

} // namespace atta::parallel
