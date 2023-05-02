//--------------------------------------------------
// Atta Memory Module
// allocator.h
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATOR_H
#define ATTA_MEMORY_ALLOCATOR_H

namespace atta::memory {

class Allocator {
  public:
    // Allocate memory
    Allocator(uint64_t size);
    // Use existing allocated memory
    Allocator(uint8_t* memory, uint64_t size);
    virtual ~Allocator();

    // How many bytes to allocate
    // size: number of bytes
    // align: byte alignment
    virtual void* allocBytes(size_t size, size_t align = 0) = 0;

    // Free bytes at the pointer
    // ptr: ptr at the start
    // size: number of bytes
    // align: byte alignment
    virtual void freeBytes(void* ptr, size_t size, size_t align = 0) = 0;

    const uint8_t* getMemory() { return _memory; }
    uint64_t getSize() { return _size; }

    bool owns(const void* ptr); // Check if pointer is inside this allocator memory

  protected:
    uint8_t* _memory;
    uint64_t _size;
    bool _shouldFree; // True if _memory was allocated by this allocator
};

} // namespace atta::memory

#endif // ATTA_MEMORY_ALLOCATOR_H
