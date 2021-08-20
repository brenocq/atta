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

		// How many objects to allocate
		//template<typename T>
		//virtual T* allocate(size_t size = 1) = 0;

		//// Free object at the pointer
		//template<typename T>
		//virtual void free(T* object) = 0;
	};
}

#endif// ATTA_MEMORY_ALLOCATOR_H
