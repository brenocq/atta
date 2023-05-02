//--------------------------------------------------
// Atta Memory Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_INTERFACE_H
#define ATTA_MEMORY_INTERFACE_H

#include <atta/memory/allocator.h>
#include <atta/utils/stringId.h>

namespace atta::memory {

// It is necessary to register the allocators to define its
// type (stack, pool, malloc,  ...) and parameters (size, ...)
void registerAllocator(StringId sid, Allocator* alloc);

// After registered, it is possible to get a pointer to the allocator
template <typename T = Allocator>
T* getAllocator(StringHash hash);

// Used by allocatedObject to store pointer to the hash table value and
// reduce number of unordered_map accesses
template <typename T = Allocator>
T** getAllocatorPtr(StringHash hash);

std::string getAllocatorName(Allocator* alloc);

const std::unordered_map<StringHash, Allocator*>& getAllocators();

} // namespace atta::memory

#include <atta/memory/manager.h>

namespace atta::memory {

template <typename T>
T* getAllocator(StringHash hash) {
    return static_cast<T*>(Manager::getInstance().getAllocatorImpl(hash));
}

template <typename T>
T** getAllocatorPtr(StringHash hash) {
    return static_cast<T**>(Manager::getInstance().getAllocatorPtrImpl(hash));
}

} // namespace atta::memory

#endif // ATTA_MEMORY_INTERFACE_H
