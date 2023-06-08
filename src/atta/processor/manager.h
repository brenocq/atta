//--------------------------------------------------
// Atta Processor Module
// manager.h
// Date: 2023-02-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PROCESSOR_MANAGER_H
#define ATTA_PROCESSOR_MANAGER_H

#include <atta/event/event.h>
#include <atta/processor/processors/cpuParallel.h>
#include <atta/processor/processors/cpuSerial.h>
#include <atta/processor/processors/gpu.h>

namespace atta::processor {

class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();

    friend void start();
    friend void pause();
    friend void resume();
    friend void stop();

    friend void readData();
    friend void writeData();

    friend void setType(Type type);
    friend Type getType();

    friend State getState();

    friend void setDt(float dt);
    friend float getDt();
    friend float getTime();

    friend std::shared_ptr<Processor> getProcessor();
    friend std::shared_ptr<CpuSerial> getCpuSerialProcessor();
    friend std::shared_ptr<CpuParallel> getCpuParallelProcessor();
    friend std::shared_ptr<Gpu> getGpuProcessor();

  private:
    void startUpImpl();
    void shutDownImpl();

    void start();
    void pause();
    void resume();
    void stop();

    void setTypeImpl(Type type);
    Type getTypeImpl();

    State getStateImpl();

    std::shared_ptr<Processor> getProcessorImpl();
    std::shared_ptr<CpuSerial> getCpuSerialProcessorImpl();
    std::shared_ptr<CpuParallel> getCpuParallelProcessorImpl();
    std::shared_ptr<Gpu> getGpuProcessorImpl();

    State _state;
    float _dt;

    std::shared_ptr<Processor> _processor;     ///< Selected processor
    std::shared_ptr<CpuSerial> _cpuSerial;     ///< CPU serial processor
    std::shared_ptr<CpuParallel> _cpuParallel; ///< CPU parallel processor
    std::shared_ptr<Gpu> _gpu;                 ///< GPU processor
};

} // namespace atta::processor

#endif // ATTA_PROCESSOR_MANAGER_H
