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

void parallelFor(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) { Manager::getInstance().parallelForImpl(start, end, func); }

void setDevice(Device::Type type) { Manager::getInstance().setDeviceImpl(type); }
Device::Type getDevice() { return Manager::getInstance().getDeviceImpl(); }

} // namespace atta::parallel
