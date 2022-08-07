//--------------------------------------------------
// Atta Memory Module
// bitmapAllocator.h
// Date: 2021-08-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATORS_BITMAP_ALLOCATOR_H
#define ATTA_MEMORY_ALLOCATORS_BITMAP_ALLOCATOR_H
#include <atta/memory/allocator.h>

namespace atta
{
    // Allocator for variable-size objects, free bytes are known using a bitmap
    // The first memory bytes are used by the bitmap
    //
    // Each bit in the bitmap section corresponds to one byte
    // 1 -> allocated | 0 -> free
    class BitmapAllocator final : public Allocator
    {
    public:
        // Allocate heap memory
        BitmapAllocator(uint64_t size, uint32_t blockSize = 1);
        // Use already allocated memory
        BitmapAllocator(uint8_t* memory, uint64_t size, uint32_t blockSize = 1);

        // Simplified alloc/free
        template<typename T>
        T* alloc(size_t size = 1);
        template<typename T>
        T* allocAtIndex(int64_t index = -1, size_t size = 1);
        template<typename T>
        void free(T* ptr, size_t size = 1);

        // Default alloc/free
        void* allocBytes(size_t size, size_t align) override;
        void freeBytes(void* ptr, size_t size, size_t align) override;

        // Helpers
        void clear();

        uint64_t getIndex(void* block);
        void* getBlock(uint64_t index);
        template<typename T>
        T* getBlock(uint64_t index);
        bool getBlockBit(uint64_t index);

        size_t getDataSize() const { return _dataSize; }
        size_t getBlockSize() const { return _blockSize; }

    private:
        void init();
        void setBlockBit(uint64_t index, bool bit);

        uint8_t* _dataStart;
        size_t _dataSize;
        size_t _blockSize;
        size_t _current;// Position to start free space search
    };
}

#include <atta/memory/allocators/bitmapAllocator.inl>
#endif// ATTA_MEMORY_ALLOCATORS_BITMAP_ALLOCATOR_H
