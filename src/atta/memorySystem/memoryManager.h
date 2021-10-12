//--------------------------------------------------
// Atta Memory System
// memoryManager.h
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_SYSTEM_MEMORY_MANAGER_H
#define ATTA_MEMORY_SYSTEM_MEMORY_MANAGER_H
#include <atta/memorySystem/allocator.h>
#include <atta/core/stringId.h>

namespace atta
{
    class MemoryManager final
    {
    public:
        static MemoryManager& getInstance()
        {
            static MemoryManager instance;
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
}

#include <atta/memorySystem/memoryManager.inl>
#endif// ATTA_MEMORY_SYSTEM_MEMORY_MANAGER_H
