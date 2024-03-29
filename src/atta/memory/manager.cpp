//--------------------------------------------------
// Atta Memory Module
// manager.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/interface.h>
#include <atta/memory/manager.h>

namespace atta::memory {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

Allocator* Manager::getAllocatorImpl(StringHash hash) {
    auto allocator = _allocators.find(hash);
    if (allocator == _allocators.end()) {
        ASSERT(false, "Trying to use allocator that was never registered [w]$0[]", hash);
        return nullptr;
    } else
        return allocator->second;
}

Allocator** Manager::getAllocatorPtrImpl(StringHash hash) {
    auto allocator = _allocators.find(hash);
    if (allocator == _allocators.end()) {
        ASSERT(false, "Trying to use allocator that was never registered");
        return nullptr;
    } else
        return &allocator->second;
}

void Manager::registerAllocatorImpl(StringHash hash, Allocator* alloc) {
    // Just replaces the pointer, does not delete the allocator
    _allocators[hash] = alloc;
}

} // namespace atta::memory
