// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_MEMORY_INTERFACE_H
#define ATTA_MEMORY_INTERFACE_H

#include <atta/memory/allocator.h>
#include <atta/utils/stringId.h>

namespace atta::memory {

// It is necessary to register the allocators to define its
// type (stack, pool, malloc,  ...) and parameters (size, ...)
void registerAllocator(StringHash hash, Allocator* alloc);

// After registered, it is possible to get a pointer to the allocator
template <typename T = Allocator>
T* getAllocator(StringHash hash);

// Used by allocatedObject to store pointer to the hash table value and
// reduce number of unordered_map accesses
template <typename T = Allocator>
T** getAllocatorPtr(StringHash hash);

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
