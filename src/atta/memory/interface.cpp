//--------------------------------------------------
// Atta Memory Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/interface.h>

namespace atta::memory {

void registerAllocator(StringId sid, Allocator* alloc) { Manager::getInstance().registerAllocatorImpl(sid, alloc); }

std::string getAllocatorName(Allocator* alloc) { return Manager::getInstance().getAllocatorNameImpl(alloc); }

const std::unordered_map<StringHash, Allocator*>& getAllocators() { return Manager::getInstance().getAllocatorsImpl(); }

} // namespace atta::memory
