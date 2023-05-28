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

    void run(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) override;

    /**
     * @brief Run scripts sequentially
     *
     * @tparam Script Derived script class
     * @param entity First clone
     * @param dt Simulation dt
     * @param num Number of entities with sequential id to run
     */
    template <typename Script>
    void run(cmp::Entity entity, float dt, uint32_t num);
};

} // namespace atta::parallel

#include <atta/parallel/devices/serialDevice.inl>

#endif // ATTA_PARALLEL_DEVICES_CPU_DEVICE_H
