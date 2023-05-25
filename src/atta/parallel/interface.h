//--------------------------------------------------
// Atta Parallel Module
// interface.h
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_INTERFACE_H
#define ATTA_PARALLEL_INTERFACE_H

#include <atta/parallel/manager.h>

namespace atta::parallel {

void startUp();
void shutDown();

void run(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func);

/**
 * @brief Run scripts
 *
 * @tparam Script Derived script class
 * @param entity First clone
 * @param dt Simulation dt
 * @param num Number of entities with sequential id to run
 */
template <typename Script>
void run(cmp::Entity entity, float dt, uint32_t num);

void setDeviceType(Device::Type type);
Device::Type getDeviceType();

std::shared_ptr<Device> getDevice();
std::shared_ptr<SerialDevice> getSerialDevice();
std::shared_ptr<CpuDevice> getCpuDevice();
std::shared_ptr<GpuDevice> getGpuDevice();

} // namespace atta::parallel

#include <atta/parallel/interface.inl>

#endif // ATTA_PARALLEL_INTERFACE_H
