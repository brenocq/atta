//--------------------------------------------------
// Atta Processor Module
// serial.h
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_PROCESSORS_SERIAL_H
#define ATTA_PARALLEL_PROCESSORS_SERIAL_H
#include <atta/processor/processors/processor.h>

namespace atta::processor {

class Serial : public Processor {
  public:
    Serial();
    ~Serial();

    void start() override;
    void pause() override;
    void resume() override;
    void stop() override;
};

} // namespace atta::processor

#endif // ATTA_PARALLEL_PROCESSORS_SERIAL_H
