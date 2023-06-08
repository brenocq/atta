//--------------------------------------------------
// Atta Processor Module
// cpuSerial.h
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_PROCESSORS_CPU_SERIAL_H
#define ATTA_PARALLEL_PROCESSORS_CPU_SERIAL_H
#include <atta/processor/processors/processor.h>

namespace atta::processor {

class CpuSerial : public Processor {
  public:
    CpuSerial();
    ~CpuSerial();

  private:
    void startThread() override;

    void loop();
};

} // namespace atta::processor

#endif // ATTA_PARALLEL_PROCESSORS_CPU_SERIAL_H
