//--------------------------------------------------
// Atta Memory
// mallocAllocator.cpp
// Date: 2021-08-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/allocators/mallocAllocator.h>

namespace atta
{
	MallocAllocator::MallocAllocator()
	{
	}

	MallocAllocator::~MallocAllocator()
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
