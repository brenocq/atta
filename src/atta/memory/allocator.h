//--------------------------------------------------
// Atta Memory
// allocator.h
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATOR_H
#define ATTA_MEMORY_ALLOCATOR_H

namespace atta
{
	class Allocator
	{
	public:
		virtual ~Allocator() = default;

		// How many bytes to allocate
		// size: number of bytes
		// align: byte alignment
		virtual void* allocBytes(size_t size, size_t align = 0) = 0;

		// Free object at the pointer
		// ptr: ptr at the start
		// size: number of bytes
		// align: byte alignment
		virtual void freeBytes(void* ptr, size_t size, size_t align = 0) = 0;
	};
}

#endif// ATTA_MEMORY_ALLOCATOR_H
