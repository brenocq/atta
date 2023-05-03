//--------------------------------------------------
// Atta Parallel Module
// interface.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/interface.h>

namespace atta::parallel {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }

void run(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) {
    PROFILE();
    Manager::getInstance().runImpl(start, end, func);
}

void run(scr::Script* script, cmp::Entity entity, float dt, uint32_t num) {
    PROFILE();
    Manager::getInstance().runImpl(script, entity, dt, num);
}

void setDeviceType(Device::Type type) { Manager::getInstance().setDeviceTypeImpl(type); }
Device::Type getDeviceType() { return Manager::getInstance().getDeviceTypeImpl(); }
std::shared_ptr<Device> getDevice() { return Manager::getInstance().getDeviceImpl(); }
std::shared_ptr<SerialDevice> getSerialDevice() { return Manager::getInstance().getSerialDeviceImpl(); }
std::shared_ptr<CpuDevice> getCpuDevice() { return Manager::getInstance().getCpuDeviceImpl(); }
std::shared_ptr<GpuDevice> getGpuDevice() { return Manager::getInstance().getGpuDeviceImpl(); }

} // namespace atta::parallel
