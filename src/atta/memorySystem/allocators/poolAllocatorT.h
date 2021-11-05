//--------------------------------------------------
// Atta Memory System
// poolAllocatorT.h
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATORT_H
#define ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATORT_H
#include <atta/memorySystem/allocators/poolAllocator.h>

namespace atta
{
    template<typename T>
    class PoolAllocatorT final : public PoolAllocator
    {
    public:
        // If blockAlign is set to zero, no alignment
        // Allocate heap memory
        PoolAllocatorT(size_t countBlocks, size_t blockAlign = 0);
        // Use already allocated memory
        PoolAllocatorT(uint8_t* memory, size_t countBlocks, size_t blockAlign = 0);

        // Simplified typed alloc/free
        T* alloc(size_t count = 1);
        void free(T* ptr, size_t count = 1);

        // Typed access
        uint64_t getIndex(T* block);
        T* getBlock(uint64_t index);// Return the block even if it is free, this can broke the pool allocator
    };
}

#include <atta/memorySystem/allocators/poolAllocatorT.inl>
#endif// ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATORT_H
