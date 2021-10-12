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
        return static_cast<T*>(allocBytes(size*sizeof(T), sizeof(T)));
    }

    template<typename T>
    void BitmapAllocator::free(T* ptr, size_t size)
    {
        freeBytes(reinterpret_cast<void*>(ptr), sizeof(T)*size, sizeof(T));
    }
}
