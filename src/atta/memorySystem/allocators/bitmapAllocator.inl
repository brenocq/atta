//--------------------------------------------------
// Atta Memory System
// bitmapAllocator.inl
// Date: 2021-08-27
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    template<typename T>
    T* BitmapAllocator::alloc(size_t size)
    {
        return static_cast<T*>(allocBytes(size*_blockSize, _blockSize));
    }

    template<typename T>
    void BitmapAllocator::free(T* ptr, size_t size)
    {
        freeBytes(reinterpret_cast<void*>(ptr), size*_blockSize, _blockSize);
    }

    template<typename T>
    T* BitmapAllocator::getBlock(uint64_t index)
    {
        return static_cast<T*>(getBlock(index));
    }
}
