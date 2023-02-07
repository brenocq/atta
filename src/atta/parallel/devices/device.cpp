//--------------------------------------------------
// Atta Parallel Module
// device.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/devices/device.h>

namespace atta::parallel {

Device::Device(Type type) : _type(type) {}

Device::Type Device::getType() const { return _type; }

} // namespace atta::parallel
