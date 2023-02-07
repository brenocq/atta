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
    _cpuDevice = std::make_shared<CpuDevice>();
    _device = _cpuDevice;
}

void Manager::shutDownImpl() {
    _device.reset();
    _cpuDevice.reset();
}

void Manager::parallelForImpl(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) { _device->parallelFor(start, end, func); }

void Manager::setDeviceImpl(Device::Type type) {
    switch(type) {
        case Device::Type::CPU:
            _device = _cpuDevice;
            break;
        default:
            LOG_WARN("parallle::Manager", "This device is not supported yet");
    }
}

Device::Type Manager::getDeviceImpl() {
    return _device->getType();
}

} // namespace atta::parallel
