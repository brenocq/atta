//--------------------------------------------------
// Atta Memory Module
// poolAllocator.h
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H
#define ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H
#include <atta/memory/allocator.h>

namespace atta::memory {

class PoolAllocator : public Allocator {
  public:
    // If blockAlign is set to zero, no alignment
    // The blocksize can be greater than the specified because also uses the same position to store a pointer
    // Allocate heap memory
    PoolAllocator(size_t countBlocks, size_t minBlockSize, size_t blockAlign = 0);
    // Use already allocated memory
    PoolAllocator(uint8_t* memory, size_t countBlocks, size_t minBlockSize, size_t blockAlign = 0);

    // Simplified alloc/free
    void* alloc(size_t count = 1);
    void free(void* ptr, size_t count = 1);

    void* allocBytes(size_t size, size_t align) override;
    void freeBytes(void* ptr, size_t size, size_t align) override;

    void clear();
    uint64_t getIndex(void* block);
    void* getBlock(uint64_t index); // Return the block even if it is free, this can broke the pool allocator

  private:
    struct Node {
        Node* next;
    };

    size_t _blockSize;
    size_t _blockCount;
    size_t _blockAlign;
    Node* _freeList;
};

} // namespace atta::memory

#endif // ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H
