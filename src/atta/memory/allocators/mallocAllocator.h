//--------------------------------------------------
// Atta Memory
// mallocAllocator.h
// Date: 2021-08-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATORS_MALLOC_ALLOCATOR_H
#define ATTA_MEMORY_ALLOCATORS_MALLOC_ALLOCATOR_H
#include <atta/memory/allocator.h>

namespace atta
{
	// Allocator for profiling purpose only, it uses malloc() behind
	class MallocAllocator : public Allocator
	{
	public:
		MallocAllocator();
		~MallocAllocator();

		void* allocBytes(size_t size, size_t align) override;
		void freeBytes(void* ptr, size_t size, size_t align) override;

	private:
	};
}

#endif// ATTA_MEMORY_ALLOCATORS_MALLOC_ALLOCATOR_H
