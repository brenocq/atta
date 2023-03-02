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

void compute(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func);

void setDeviceType(Device::Type type);
Device::Type getDeviceType();

std::shared_ptr<Device> getDevice();
std::shared_ptr<SerialDevice> getSerialDevice();
std::shared_ptr<CpuDevice> getCpuDevice();
std::shared_ptr<GpuDevice> getGpuDevice();

} // namespace atta::parallel

#endif // ATTA_PARALLEL_INTERFACE_H
