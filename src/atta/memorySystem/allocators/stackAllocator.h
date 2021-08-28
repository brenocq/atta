//--------------------------------------------------
// Atta Memory System
// stackAllocator.h
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATORS_STACK_ALLOCATOR_H
#define ATTA_MEMORY_ALLOCATORS_STACK_ALLOCATOR_H
#include <atta/memorySystem/allocator.h>

namespace atta
{
	// Allocator to objects of variable size. To deallocate can return stack to saved marker.
	// Can free the memory using rollback or deallocate
	class StackAllocator final : public Allocator
	{
	public:
		using Marker = size_t;

		// Allocate heap memory
		StackAllocator(size_t size);
		// Use already allocated memory
		StackAllocator(uint8_t* memory, size_t size);

		// Simplified alloc/free
		template<typename T>
		T* alloc(size_t size = 1);
		template<typename T>
		void free(T* ptr, size_t size = 1);

		// Default alloc/free
		void* allocBytes(size_t size, size_t align) override;
		void freeBytes(void* ptr, size_t size, size_t align) override;

		// Return stack to marker (free up to marker)
		void rollback(Marker marker) { _current = marker; }
		// Clear entire stack
		void clear() { _current = 0; }

		Marker getMarker() { return _current; }
		size_t getUsedMemory() { return _current; }

	private:
		size_t _current;
	};
}

#include <atta/memorySystem/allocators/stackAllocator.inl>
#endif// ATTA_MEMORY_ALLOCATORS_STACK_ALLOCATOR_H