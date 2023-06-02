//--------------------------------------------------
// Atta Processor Module
// gpu.cpp
// Date: 2023-03-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/processor/processors/gpu.h>
#include <atta/utils/cuda.h>
#include <cuda_runtime.h>

namespace atta::processor {

Gpu::Gpu() : Processor(Type::GPU) {}

Gpu::~Gpu() {}

void Gpu::start() { _state = State::RUNNING; }
void Gpu::pause() { _state = State::PAUSED; }
void Gpu::resume() { _state = State::RUNNING; }
void Gpu::stop() { _state = State::IDLE; }

void Gpu::readData() {}
void Gpu::writeData() {}

} // namespace atta::processor
