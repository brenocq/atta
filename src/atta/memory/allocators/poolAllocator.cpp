//--------------------------------------------------
// Atta Memory Module
// poolAllocator.cpp
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/allocators/poolAllocator.h>

namespace atta::memory {

PoolAllocator::PoolAllocator(size_t blockCount, size_t minBlockSize, size_t blockAlign)
    : Allocator(blockAlign == 0 ? std::max((size_t)sizeof(Node), minBlockSize) * blockCount : blockAlign * blockCount),
      _blockSize(std::max((size_t)sizeof(Node), minBlockSize)), _blockCount(blockCount), _blockAlign(blockAlign == 0 ? _blockSize : blockAlign),
      _freeList(nullptr) {
    DASSERT(_blockCount > 0, "Pool should have more than 0 blocks");
    DASSERT(_blockAlign >= _blockSize, "Block align must be equal or greater than the block size");

    // Create free list
    clear();
}

PoolAllocator::PoolAllocator(uint8_t* memory, size_t blockCount, size_t minBlockSize, size_t blockAlign)
    : Allocator(memory, blockAlign == 0 ? std::max((size_t)sizeof(Node), minBlockSize) * blockCount : blockAlign * blockCount),
      _blockSize(std::max((size_t)sizeof(Node), minBlockSize)), _blockCount(blockCount), _blockAlign(blockAlign == 0 ? _blockSize : blockAlign),
      _freeList(nullptr) {
    DASSERT(_blockCount > 0, "Pool should have more than 0 blocks");
    DASSERT(_blockAlign >= _blockSize, "Block align must be equal or greater than the block size");

    // Create free list
    clear();
}

void* PoolAllocator::alloc(size_t count) {
    DASSERT(count == 1, "Allocation with more than one block is not supported yet");
    return reinterpret_cast<void*>(allocBytes(_blockSize * count, _blockSize));
}

void PoolAllocator::free(void* ptr, size_t count) {
    DASSERT(count == 1, "Free with more than one block is not supported yet");

    freeBytes(ptr, _blockSize, _blockSize);
}

void PoolAllocator::clear() {
    uint8_t* curr = Allocator::_memory;
    uint8_t* next = curr + _blockSize;
    _freeList = reinterpret_cast<Node*>(curr);
    for (size_t i = 0; i < _blockCount - 1; i++) {
        Node* currNode = reinterpret_cast<Node*>(curr);
        Node* nextNode = reinterpret_cast<Node*>(next);
        currNode->next = nextNode;
        curr += _blockSize;
        next += _blockSize;
    }
    Node* tailNode = reinterpret_cast<Node*>(curr);
    tailNode->next = nullptr;
}

uint64_t PoolAllocator::getIndex(void* block) { return (reinterpret_cast<uint8_t*>(block) - _memory) / _blockSize; }

void* PoolAllocator::getBlock(uint64_t index) { return reinterpret_cast<void*>(_memory + index * _blockSize); }

void* PoolAllocator::allocBytes(size_t size, size_t align) {
    DASSERT(size == _blockSize, "AllocBytes with more than one block is not supported yet");

    // Check if there are block availables
    if (_freeList == nullptr)
        return nullptr;

    // Update freelist and return block
    Node* node = _freeList;
    _freeList = node->next;
    return reinterpret_cast<void*>(node);
}

void PoolAllocator::freeBytes(void* ptr, size_t size, size_t align) {
    DASSERT(size == _blockSize, "FreeBytes with more than one block is not supported yet");

    Node* node = static_cast<Node*>(ptr);
    Node* last = _freeList;

    // Add block to end of the list
    node->next = last;
    // Update list root
    _freeList = node;
}

} // namespace atta::memory
