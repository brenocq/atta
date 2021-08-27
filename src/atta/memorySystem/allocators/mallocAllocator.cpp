//--------------------------------------------------
// Atta Memory System
// mallocAllocator.cpp
// Date: 2021-08-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memorySystem/allocators/mallocAllocator.h>

namespace atta
{
	MallocAllocator::MallocAllocator():
		Allocator(nullptr, 0)
	{

	}

	void* MallocAllocator::allocBytes(size_t size, size_t align)
	{
		return malloc(size);
	}

	void MallocAllocator::freeBytes(void* ptr, size_t size, size_t align)
	{
		free(ptr);
	}
}
