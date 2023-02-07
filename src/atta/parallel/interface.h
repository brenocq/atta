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

void parallelFor(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func);

void setDevice(Device::Type type);
Device::Type getDevice();

} // namespace atta::parallel

#endif // ATTA_PARALLEL_INTERFACE_H
