//--------------------------------------------------
// Atta Processor Module
// interface.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/processor/interface.h>

namespace atta::processor {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }

void start() { Manager::getInstance().start(); }
void pause() { Manager::getInstance().pause(); }
void resume() { Manager::getInstance().resume(); }
void stop() { Manager::getInstance().stop(); }

void readData() { getProcessor()->readData(); }
void writeData() { getProcessor()->writeData(); }

void setType(Type type) { Manager::getInstance().setTypeImpl(type); }
Type getType() { return Manager::getInstance().getTypeImpl(); }

State getState() { return Manager::getInstance().getStateImpl(); }

void setDt(float dt) { Manager::getInstance()._dt = dt; }
float getDt() { return Manager::getInstance()._dt; }

float getTime() { return Manager::getInstance()._processor->getStepCount() * getDt(); }

std::shared_ptr<Processor> getProcessor() { return Manager::getInstance().getProcessorImpl(); }
std::shared_ptr<CpuSerial> getCpuSerialProcessor() { return Manager::getInstance().getCpuSerialProcessorImpl(); }
std::shared_ptr<CpuParallel> getCpuParallelProcessor() { return Manager::getInstance().getCpuParallelProcessorImpl(); }
std::shared_ptr<Gpu> getGpuProcessor() { return Manager::getInstance().getGpuProcessorImpl(); }

} // namespace atta::processor
