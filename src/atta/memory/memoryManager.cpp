//--------------------------------------------------
// Atta Memory Module
// memoryManager.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/memoryManager.h>

namespace atta
{
    void MemoryManager::registerAllocator(StringHash hash, Allocator* alloc)
    {
        getInstance().registerAllocatorImpl(hash, alloc);
    }

    Allocator* MemoryManager::getAllocatorImpl(StringHash hash)
    {
        auto allocator = _allocators.find(hash);
        if(allocator == _allocators.end())
        {
            ASSERT(false, "Trying to use allocator that was never registered [w]$0[]", hash);
            return nullptr;
        }
        else
            return allocator->second;
    }

    Allocator** MemoryManager::getAllocatorPtrImpl(StringHash hash)
    {
        auto allocator = _allocators.find(hash);
        if(allocator == _allocators.end())
        {
            ASSERT(false, "Trying to use allocator that was never registered");
            return nullptr;
        }
        else
            return &allocator->second;
    }

    void MemoryManager::registerAllocatorImpl(StringHash hash, Allocator* alloc)
    {
        //if(_allocators.find(hash) != _allocators.end())
        //{
        //	// Delete old allocator
        //	//delete _allocators[hash];
        //	// Add new allocator
        //	_allocators[hash] = alloc;
        //}
        //else
        //{
        //}

        // Just replaces the pointer, does not delete the allocator
        _allocators[hash] = alloc;
    }
}

