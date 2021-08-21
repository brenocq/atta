//--------------------------------------------------
// Atta Memory
// stackAllocator.h
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATORS_STACK_ALLOCATOR_H
#define ATTA_MEMORY_ALLOCATORS_STACK_ALLOCATOR_H
#include <atta/memory/allocator.h>

namespace atta
{
	// Allocator to objects of variable size. To deallocate can return stack to saved marker.
	// Can free the memory using rollback or deallocate
	class StackAllocator : public Allocator
	{
	public:
		using Marker = size_t;

		StackAllocator(size_t size);
		~StackAllocator();

		// Simplified alloc/free
		template<typename T>
		T* alloc(size_t size = 1);
		template<typename T>
		void free(T* ptr);
		template<typename T>
		void free(T* ptr, size_t size);

		// Default alloc/free
		void* allocBytes(size_t size, size_t align) override;
		void freeBytes(void* ptr, size_t size, size_t align) override;

		// Return stack to the marker (free up to the marker)
		void rollback(Marker marker) { _current = marker; }
		// Clear entire stack
		void clear() { _current = 0; }

		Marker getMarker() { return _current; }
		size_t getUsedMemory() { return _current; }
		size_t getTotalMemory() { return _size; }

	private:
		size_t _current;
		size_t _size;
		uint8_t* _memory;
	};
}

#include <atta/memory/allocators/stackAllocator.inl>
#endif// ATTA_MEMORY_ALLOCATORS_STACK_ALLOCATOR_H
