//--------------------------------------------------
// Atta Parallel Module
// device.h
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_DEVICES_DEVICE_H
#define ATTA_PARALLEL_DEVICES_DEVICE_H

namespace atta::parallel {

class Device {
  public:
    enum class Type {
        SERIAL = 0, CPU, GPU, CLUSTER
    };

    Device(Type type);

    virtual void compute(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) = 0;

    Type getType() const;

  protected:
    const Type _type;
};

} // namespace atta::parallel

#endif // ATTA_PARALLEL_DEVICES_DEVICE_H
