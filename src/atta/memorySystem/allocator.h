//--------------------------------------------------
// Atta Memory System
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
		// Allocate memory
		Allocator(size_t size);
		// Use existing allocated memory
		Allocator(uint8_t* memory, size_t size);
		virtual ~Allocator();

		// How many bytes to allocate
		// size: number of bytes
		// align: byte alignment
		virtual void* allocBytes(size_t size, size_t align = 0) = 0;

		// Free bytes at the pointer
		// ptr: ptr at the start
		// size: number of bytes
		// align: byte alignment
		virtual void freeBytes(void* ptr, size_t size, size_t align = 0) = 0;

		size_t getSize() { return _size; }

	protected:
		uint8_t* _memory;
		size_t _size;
		bool _shouldFree;// True if _memory was allocated by this allocator
	};
}

#endif// ATTA_MEMORY_ALLOCATOR_H