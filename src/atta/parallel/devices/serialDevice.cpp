//--------------------------------------------------
// Atta Parallel Module
// serialDevice.cpp
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/devices/serialDevice.h>

namespace atta::parallel {

SerialDevice::SerialDevice() : Device(Type::SERIAL) {}

SerialDevice::~SerialDevice() {}

void SerialDevice::compute(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) {
    for (uint32_t i = start; i < end; i++)
        func(i);
}

} // namespace atta::parallel
