//--------------------------------------------------
// Atta Processor Module
// interface.h
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PROCESSOR_INTERFACE_H
#define ATTA_PROCESSOR_INTERFACE_H

#include <atta/processor/base.h>
#include <atta/processor/manager.h>

namespace atta::processor {

void startUp();
void shutDown();

void start();
void pause();
void resume();
void stop();

void readData();
void writeData();

void setType(Type type);
Type getType();

std::shared_ptr<Processor> getProcessor();
std::shared_ptr<Serial> getSerialProcessor();
std::shared_ptr<Cpu> getCpuProcessor();
std::shared_ptr<Gpu> getGpuProcessor();

} // namespace atta::processor

#endif // ATTA_PROCESSOR_INTERFACE_H
