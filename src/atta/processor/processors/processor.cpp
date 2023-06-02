//--------------------------------------------------
// Atta Processor Module
// processor.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/processor/processors/processor.h>

namespace atta::processor {

Processor::Processor(Type type) : _type(type), _state(State::IDLE) {}

Type Processor::getType() const { return _type; }

State Processor::getState() const { return _state; }

} // namespace atta::processor
