//--------------------------------------------------
// Atta Memory
// poolAllocator.h
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H
#define ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H
#include <atta/memory/allocator.h>

namespace atta
{
	class PoolAllocator : public Allocator
	{
	public:
		PoolAllocator(size_t size);
		~PoolAllocator();
	};
}

#endif// ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H
