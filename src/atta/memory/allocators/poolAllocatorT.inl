//--------------------------------------------------
// Atta Memory Module
// poolAllocatorT.inl
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    template <typename T>
    PoolAllocatorT<T>::PoolAllocatorT(size_t countBlocks, size_t blockAlign):
        PoolAllocator(countBlocks, sizeof(T), blockAlign)
    {
    }

    template <typename T>
    PoolAllocatorT<T>::PoolAllocatorT(uint8_t* memory, size_t countBlocks, size_t blockAlign):
        PoolAllocator(memory, countBlocks, sizeof(T), blockAlign)
    {
    }

    // Simplified typed alloc/free
    template <typename T>
    T* PoolAllocatorT<T>::alloc(size_t count)
    {
        return static_cast<T*>(PoolAllocator::alloc(count));
    }

    template <typename T>
    void PoolAllocatorT<T>::free(T* ptr, size_t count)
    { 
        PoolAllocator::free(reinterpret_cast<void*>(ptr), count);
    }

    template <typename T>
    uint64_t PoolAllocatorT<T>::getIndex(T* block)
    {
        return PoolAllocator::getIndex(reinterpret_cast<void*>(block));
    }

    template <typename T>
    T* PoolAllocatorT<T>::getBlock(uint64_t index)
    {
        return static_cast<T*>(PoolAllocator::getBlock(index));
    }
}
