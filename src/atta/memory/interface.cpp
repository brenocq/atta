//--------------------------------------------------
// Atta Memory Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/interface.h>

namespace atta::memory {

void registerAllocator(StringHash hash, Allocator* alloc) { Manager::getInstance().registerAllocatorImpl(hash, alloc); }

} // namespace atta::memory
