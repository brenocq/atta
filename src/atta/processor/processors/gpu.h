//--------------------------------------------------
// Atta Processor Module
// gpu.h
// Date: 2023-03-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PROCESSOR_PROCESSORS_GPU_H
#define ATTA_PROCESSOR_PROCESSORS_GPU_H
#include <atta/processor/processors/processor.h>

namespace atta::processor {

class Gpu : public Processor {
  public:
    Gpu();
    ~Gpu();

    void start() override;
    void pause() override;
    void resume() override;
    void stop() override;

    void readData() override;
    void writeData() override;
};

} // namespace atta::processor

#endif // ATTA_PROCESSOR_PROCESSORS_GPU_H
