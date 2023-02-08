//--------------------------------------------------
// Atta Parallel Module
// serialDevice.h
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_DEVICES_SERIAL_DEVICE_H
#define ATTA_PARALLEL_DEVICES_SERIAL_DEVICE_H

#include <atta/parallel/devices/device.h>

namespace atta::parallel {

class SerialDevice : public Device {
  public:
    SerialDevice();
    ~SerialDevice();

    void compute(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) override;
};

} // namespace atta::parallel

#endif // ATTA_PARALLEL_DEVICES_CPU_DEVICE_H
