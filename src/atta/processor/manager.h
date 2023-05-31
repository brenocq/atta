//--------------------------------------------------
// Atta Processor Module
// manager.h
// Date: 2023-02-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PROCESSOR_MANAGER_H
#define ATTA_PROCESSOR_MANAGER_H

#include <atta/event/event.h>
#include <atta/processor/processors/cpu.h>
#include <atta/processor/processors/gpu.h>
#include <atta/processor/processors/serial.h>

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

    friend std::shared_ptr<Processor> getProcessor();
    friend std::shared_ptr<Serial> getSerialProcessor();
    friend std::shared_ptr<Cpu> getCpuProcessor();
    friend std::shared_ptr<Gpu> getGpuProcessor();

  private:
    void startUpImpl();
    void shutDownImpl();

    void setTypeImpl(Type type);
    Type getTypeImpl();

    std::shared_ptr<Processor> getProcessorImpl();
    std::shared_ptr<Serial> getSerialProcessorImpl();
    std::shared_ptr<Cpu> getCpuProcessorImpl();
    std::shared_ptr<Gpu> getGpuProcessorImpl();

    void onSimulationStateChange(event::Event& event);

    std::shared_ptr<Processor> _processor; ///< Selected processor
    std::shared_ptr<Serial> _serial;       ///< Serial processor
    std::shared_ptr<Cpu> _cpu;             ///< CPU processor
    std::shared_ptr<Gpu> _gpu;             ///< GPU processor
};

} // namespace atta::processor

#endif // ATTA_PROCESSOR_MANAGER_H
