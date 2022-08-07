//--------------------------------------------------
// Atta Memory Module
// stackAllocator.inl
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::memory {
template <typename T>
T* StackAllocator::alloc(size_t size) {
    return static_cast<T*>(allocBytes(size * sizeof(T), sizeof(T)));
}

template <typename T>
void StackAllocator::free(T* ptr, size_t size) {
    freeBytes(reinterpret_cast<void*>(ptr), sizeof(T) * size, sizeof(T));
}
} // namespace atta::memory
