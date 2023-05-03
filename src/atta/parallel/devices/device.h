//--------------------------------------------------
// Atta Parallel Module
// device.h
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_DEVICES_DEVICE_H
#define ATTA_PARALLEL_DEVICES_DEVICE_H
#include <atta/component/interface.h>
#include <atta/script/interface.h>

namespace atta::parallel {

class Device {
  public:
    enum Type { SERIAL = 0, CPU, GPU, CLUSTER };

    Device(Type type);

    virtual void run(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) = 0;
    virtual void run(scr::Script* script, cmp::Entity entity, float dt, uint32_t num) = 0;

    Type getType() const;

  protected:
    const Type _type;
};

} // namespace atta::parallel

#endif // ATTA_PARALLEL_DEVICES_DEVICE_H
