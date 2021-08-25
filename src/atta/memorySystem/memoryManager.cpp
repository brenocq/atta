//--------------------------------------------------
// Atta Memory
// memoryManager.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memorySystem/memoryManager.h>

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
			ASSERT(false, "Trying to use allocator that was never registered");
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
		if(_allocators.find(hash) != _allocators.end())
		{
			// Delete old allocator
			delete _allocators[hash];
			_allocators[hash] = nullptr;
			// Add new allocator
			_allocators[hash] = alloc;
		}
		else
		{
			_allocators[hash] = alloc;
		}
	}
}

