//--------------------------------------------------
// Atta Memory Module
// manager.h
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_MEMORY_MANAGER_H
#define ATTA_MEMORY_MEMORY_MANAGER_H
#include <atta/memory/allocator.h>
#include <atta/utils/stringId.h>

namespace atta::memory {

class Manager final {
  public:
    static Manager& getInstance() {
        static Manager instance;
        return instance;
    }

    // It is necessary to register the allocators to define its
    // type (stack, pool, malloc,  ...) and parameters (size, ...)
    static void registerAllocator(StringHash hash, Allocator* alloc);

    // After registered, it is possible to get a pointer to the allocator
    template <typename T = Allocator>
    static T* getAllocator(StringHash hash);

    // Used by allocatedObject to store pointer to the hash table value and
    // reduce number of unordered_map accesses
    template <typename T = Allocator>
    static T** getAllocatorPtr(StringHash hash);

  private:
    Allocator* getAllocatorImpl(StringHash hash);
    Allocator** getAllocatorPtrImpl(StringHash hash);
    void registerAllocatorImpl(StringHash hash, Allocator* alloc);

    std::unordered_map<StringHash, Allocator*> _allocators;
};

} // namespace atta::memory

#include <atta/memory/manager.inl>
#endif // ATTA_MEMORY_MEMORY_MANAGER_H
