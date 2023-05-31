//--------------------------------------------------
// Atta Processor Module
// processor.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/processor/processors/processor.h>

namespace atta::processor {

Processor::Processor(Type type) : _type(type) {}

Type Processor::getType() const { return _type; }

} // namespace atta::processor
