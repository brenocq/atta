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
	class MemoryManager
	{
	public:
		MemoryManager();

	private:
		std::unordered_map<stringHash, std::unique_ptr<Allocator>> _allocators;
	};
}

#endif// ATTA_MEMORY_MEMORY_MANAGER_H
