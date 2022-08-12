//--------------------------------------------------
// Atta Memory Module
// manager.h
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_MEMORY_MANAGER_H
#define ATTA_MEMORY_MEMORY_MANAGER_H

namespace atta::memory {

class Manager final {
  public:
    static Manager& getInstance();

    friend void registerAllocator(StringHash hash, Allocator* alloc);
    template <typename T>
    friend T* getAllocator(StringHash hash);
    template <typename T>
    friend T** getAllocatorPtr(StringHash hash);

  private:
    Allocator* getAllocatorImpl(StringHash hash);
    Allocator** getAllocatorPtrImpl(StringHash hash);
    void registerAllocatorImpl(StringHash hash, Allocator* alloc);

    std::unordered_map<StringHash, Allocator*> _allocators;
};

} // namespace atta::memory

#endif // ATTA_MEMORY_MEMORY_MANAGER_H
