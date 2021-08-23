//--------------------------------------------------
// Atta Memory
// memoryManager.h
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_MEMORY_MANAGER_H
#define ATTA_MEMORY_MEMORY_MANAGER_H
#include <atta/memory/allocator.h>
#include <atta/core/stringId.h>

namespace atta
{
	class MemoryManager final
	{
	public:
		static MemoryManager& getInstance() {
			static MemoryManager instance;
			return instance;
		}

		// After registering, it is possible to get a pointer to it
		template <typename T>
		static std::shared_ptr<T> getAllocator(StringHash hash);

		// It is necessary to register the allocators to define its 
		// type (stack, general, pool, ...) and parameters (size, ...)
		static void registerAllocator(StringHash hash, std::shared_ptr<Allocator> alloc);

	private:
		std::shared_ptr<Allocator> getAllocatorImpl(StringHash hash);
		void registerAllocatorImpl(StringHash hash, std::shared_ptr<Allocator> alloc);

		std::unordered_map<StringHash, std::shared_ptr<Allocator>> _allocators;
	};

}

#include <atta/memory/memoryManager.inl>
#endif// ATTA_MEMORY_MEMORY_MANAGER_H
