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

void Gpu::start() {}
void Gpu::pause() {}
void Gpu::resume() {}
void Gpu::stop() {}

void Gpu::readData() {}
void Gpu::writeData() {}

} // namespace atta::processor
