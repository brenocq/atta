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
	// After performing rollback, the application must not use the freed objects. 
	// For now, the stackAllocator can't pop objects
	class StackAllocator : public Allocator
	{
	public:
		using Marker = size_t;

		StackAllocator(size_t size);
		~StackAllocator();

		template<typename T>
		T* allocate(size_t size = 1);
		template<typename T>
		void free(T* object);

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
