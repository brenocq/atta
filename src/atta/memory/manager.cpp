//--------------------------------------------------
// Atta Memory Module
// manager.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/manager.h>

namespace atta::memory {

void Manager::registerAllocator(StringHash hash, Allocator* alloc) { getInstance().registerAllocatorImpl(hash, alloc); }

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
    // if(_allocators.find(hash) != _allocators.end())
    //{
    //	// Delete old allocator
    //	//delete _allocators[hash];
    //	// Add new allocator
    //	_allocators[hash] = alloc;
    // }
    // else
    //{
    // }

    // Just replaces the pointer, does not delete the allocator
    _allocators[hash] = alloc;
}

} // namespace atta::memory
