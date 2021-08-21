//--------------------------------------------------
// Atta Memory
// memoryManager.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/memoryManager.h>

namespace atta
{
	void MemoryManager::registerAllocator(StringHash hash, std::shared_ptr<Allocator> alloc)
	{
		getInstance().registerAllocatorImpl(hash, alloc);
	}

	std::shared_ptr<Allocator> MemoryManager::getAllocatorImpl(StringHash hash)
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

	void MemoryManager::registerAllocatorImpl(StringHash hash, std::shared_ptr<Allocator> alloc)
	{
		ASSERT(_allocators.find(hash) == _allocators.end(), "Allocators must be registered only once");
		_allocators[hash] = alloc;
	}
}

