//--------------------------------------------------
// Atta Memory System
// allocatedObject.inl
// Date: 2021-08-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memorySystem/memoryManager.h>

namespace atta
{
    //----------------------------------//
    //---------- new operator ----------//
    //----------------------------------//
    // Throwing operator new
    template <class Object, StringHash allocatorId>
    void* AllocatedObject<Object, allocatorId>::operator new(size_t size)
    {
        void* ptr = allocBytes(size, sizeof(Object));
        if(ptr == nullptr)
            throw std::bad_alloc();
        else
            return ptr;
    }

    // Nothrow operator new
    template <class Object, StringHash allocatorId>
    void* AllocatedObject<Object, allocatorId>::operator new (std::size_t size, const std::nothrow_t& nothrow_value) noexcept
    {
        return allocBytes(size, sizeof(Object));
    }

    // Placement operator new
    template <class Object, StringHash allocatorId>
    void* AllocatedObject<Object, allocatorId>::operator new(size_t size, void* ptr) noexcept
    {
        return ptr;
    }

    //----------------------------------//
    //--------- operator new[] ---------//
    //----------------------------------//
    // Throwing operator new[]
    template <class Object, StringHash allocatorId>
    void* AllocatedObject<Object, allocatorId>::operator new[](size_t size)
    {
        void* ptr = allocBytes(size, sizeof(Object));
        if(ptr == nullptr)
            throw std::bad_alloc();
        else
            return ptr;
    }

    // Nothrow operator new[]
    template <class Object, StringHash allocatorId>
    void* AllocatedObject<Object, allocatorId>::operator new[](std::size_t size, const std::nothrow_t& nothrow_value) noexcept
    {
        return allocBytes(size, sizeof(Object));
    }

    // Placement operator new[]
    template <class Object, StringHash allocatorId>
    void* AllocatedObject<Object, allocatorId>::operator new[](size_t size, void* ptr) noexcept
    {
        return ptr;
    }

    //-------------------------------------//
    //---------- operator delete ----------//
    //-------------------------------------//
    // Ordinary operator delete
    template <class Object, StringHash allocatorId>
    void AllocatedObject<Object, allocatorId>::operator delete(void* ptr) noexcept
    {
        freeBytes(ptr, sizeof(Object), sizeof(Object));
    }

    // Nothrow operator delete
    template <class Object, StringHash allocatorId>
    void AllocatedObject<Object, allocatorId>::operator delete(void* ptr, const std::nothrow_t& nothrow_constant) noexcept
    {
        freeBytes(ptr, sizeof(Object), sizeof(Object));
    }

    // Placement operator delete
    template <class Object, StringHash allocatorId>
    void AllocatedObject<Object, allocatorId>::operator delete(void* ptr, void* voidptr2) noexcept
    {
        freeBytes(ptr, sizeof(Object), sizeof(Object));
    }

    //-------------------------------------//
    //--------- operator delete[] ---------//
    //-------------------------------------//
    // Ordinary operator delete[]
    template <class Object, StringHash allocatorId>
    void AllocatedObject<Object, allocatorId>::operator delete[](void* ptr) noexcept
    {
        freeBytes(ptr, 0, sizeof(Object));
    }

    // Nothrow operator delete[]
    template <class Object, StringHash allocatorId>
    void AllocatedObject<Object, allocatorId>::operator delete[](void* ptr, const std::nothrow_t& nothrow_constant) noexcept
    {
        freeBytes(ptr, 0, sizeof(Object));
    }

    // Placement operator delete[]
    template <class Object, StringHash allocatorId>
    void AllocatedObject<Object, allocatorId>::operator delete[](void* ptr, void* voidptr2) noexcept
    {
        freeBytes(ptr, 0, sizeof(Object));
    }

    // Ordinary operator delete[] with size
    template <class Object, StringHash allocatorId>
    void AllocatedObject<Object, allocatorId>::operator delete[](void* ptr, std::size_t size) noexcept
    {
        freeBytes(ptr, size, sizeof(Object));
    }

    // Nothrow operator delete[] with size
    template <class Object, StringHash allocatorId>
    void AllocatedObject<Object, allocatorId>::operator delete[](void* ptr, std::size_t size, const std::nothrow_t& nothrow_constant) noexcept
    {
        freeBytes(ptr, size, sizeof(Object));
    }

    //-------------------------------------//
    //--------------- common --------------//
    //-------------------------------------//
    template <class Object, StringHash allocatorId>
    // It is faster to use last allocator pointer than accessing the MemoryManager unordered_map
    Allocator* getLastAllocator()
    {
        static Allocator** allocator = MemoryManager::getAllocatorPtr<Allocator>(allocatorId);

        // Try to allocate using last used allocator
        if(allocator != nullptr && *allocator != nullptr)
            return *allocator;
        else
            return nullptr;
    }

    template <class Object, StringHash allocatorId>
    void* AllocatedObject<Object, allocatorId>::allocBytes(size_t size, size_t align)
    {
        Allocator* allocator = getLastAllocator<Object, allocatorId>();
        if(allocator != nullptr)
            return allocator->allocBytes(size, align);
        else
            return nullptr;
    }

    template <class Object, StringHash allocatorId>
    void AllocatedObject<Object, allocatorId>::freeBytes(void* ptr, size_t size, size_t align)
    {
        Allocator* allocator = getLastAllocator<Object, allocatorId>();
        if(allocator != nullptr)
            return allocator->freeBytes(ptr, size, align);
    }
}
