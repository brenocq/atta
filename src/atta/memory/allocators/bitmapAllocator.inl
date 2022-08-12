//--------------------------------------------------
// Atta Memory Module
// bitmapAllocator.inl
// Date: 2021-08-27
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::memory {

template <typename T>
T* BitmapAllocator::alloc(size_t size) {
    return static_cast<T*>(allocBytes(size * _blockSize, _blockSize));
}

template <typename T>
T* BitmapAllocator::allocAtIndex(int64_t index, size_t size) {
    if (index == -1 || getBlockBit(index))
        return nullptr;
    _current = index;
    return alloc<T>(size);
}

template <typename T>
void BitmapAllocator::free(T* ptr, size_t size) {
    freeBytes(reinterpret_cast<void*>(ptr), size * _blockSize, _blockSize);
}

template <typename T>
T* BitmapAllocator::getBlock(uint64_t index) {
    return static_cast<T*>(getBlock(index));
}

} // namespace atta::memory
