//--------------------------------------------------
// Atta Memory Module
// bitmapAllocator.cpp
// Date: 2021-08-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/allocators/bitmapAllocator.h>

namespace atta::memory {

BitmapAllocator::BitmapAllocator(uint64_t size, uint32_t blockSize) : Allocator(size), _blockSize(blockSize), _current(0) { init(); }

BitmapAllocator::BitmapAllocator(uint8_t* memory, uint64_t size, uint32_t blockSize) : Allocator(memory, size), _blockSize(blockSize), _current(0) {
    init();
}

void BitmapAllocator::init() {
    // x := Number of bytes in the bitmap
    // x + x*8*blockSize = size
    // x*(1 + 8*blockSize) = size
    // x = ceil(size/(1 + 8*blockSize))

    size_t bitmapSize = ceil(_size / (1.0f + 8 * _blockSize));
    _dataStart = _memory + bitmapSize;
    _dataSize = _size - bitmapSize;
    DASSERT(_dataSize % _blockSize == 0,
            "Data size [w]$0[] is not a multiple of block size [w]$1[], some errors may occur. Try recalculating the memory size to take into "
            "account the bitmap space",
            _dataSize, _blockSize);

    for (size_t i = 0; i < bitmapSize; i++)
        _memory[i] = 0;
}

void* BitmapAllocator::allocBytes(size_t size, size_t align) {
    DASSERT(_size > 1, "Memory size must be greater than 1");
    DASSERT(size % _blockSize == 0,
            "Trying to allocate [w]$0[] bytes, but each block have [w]$1[] bytes. The number of bytes to allocate must be a multiple of [w]$1[]",
            size, _blockSize);

    const size_t start = _current;
    const size_t numBlocks = size / _blockSize;

    size_t curr = start;
    unsigned freeCount = 0;

    do {
        // Check if it is free
        bool isFree = !getBlockBit(curr);
        if (isFree)
            freeCount++;
        else
            freeCount = 0;

        // Check can allocate
        if (freeCount == numBlocks) {
            // Found free bytes
            // Set bits as allocated
            for (size_t i = curr - numBlocks + 1; i <= curr; i++)
                setBlockBit(i, true);

            // Update _current for next search
            _current = (curr + 1) % (_dataSize / _blockSize);

            // Return pointer to first position
            return &_dataStart[curr * _blockSize - size + _blockSize];
        }

        curr++;
        // Check if need to return to first data byte
        if (curr >= _dataSize / _blockSize) {
            // Restart counter
            freeCount = 0;

            // Return to first data byte
            curr = 0;
        }
    } while (curr != start);

    return nullptr;
}

void BitmapAllocator::freeBytes(void* ptr, size_t size, size_t align) {
    DASSERT(size % _blockSize == 0,
            "Trying to deallocate [w]$0[] bytes, but each block have [w]$1[] bytes. The number of bytes to deallocate must be a multiple of [w]$1[]",
            size, _blockSize);
    uint64_t index = getIndex(ptr);

    // Try to avoid memory fragmentation
    // The next allocation will start the search from the first free block
    _current = std::min((uint64_t)_current, index);

    // XXX Can be faster if set byte to zero
    unsigned numBlocks = size / _blockSize;
    for (uint64_t i = index; i < index + numBlocks; i++)
        setBlockBit(i, false);
}

// Helpers
void BitmapAllocator::clear() {
    freeBytes(_dataStart, _dataSize, _blockSize);
    _current = 0;
}

uint64_t BitmapAllocator::getIndex(void* block) { return (static_cast<uint8_t*>(block) - _dataStart) / _blockSize; }

void* BitmapAllocator::getBlock(uint64_t index) { return getBlockBit(index) ? reinterpret_cast<void*>(_dataStart + _blockSize * index) : nullptr; }

bool BitmapAllocator::getBlockBit(uint64_t index) { return (_memory[index / 8] & (1 << (index % 8))) > 0; }

void BitmapAllocator::setBlockBit(uint64_t index, bool bit) {
    _memory[index / 8] = (_memory[index / 8] & ~(1 << (index % 8))) // Clear bit
                         + (1 << (index % 8)) * bit;                // Set bit
}

} // namespace atta::memory
