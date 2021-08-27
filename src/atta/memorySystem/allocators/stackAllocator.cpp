//--------------------------------------------------
// Atta Memory System
// stackAllocator.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memorySystem/allocators/stackAllocator.h>

namespace atta
{
	StackAllocator::StackAllocator(size_t size):
		Allocator(size), _current(0)
	{

	}

	StackAllocator::StackAllocator(uint8_t* memory, size_t size):
		Allocator(memory, size), _current(0)
	{

	}

	void* StackAllocator::allocBytes(size_t size, size_t align)
	{
		//ASSERT(align != 0, "StackAllocator::allocBytes align parameter must not be zero (aligned stack)");
		if(_current + size > _size)
			return nullptr;
		else
		{
			void* ptr = reinterpret_cast<void*>(_memory+_current);
			_current += size;
			return ptr;
		}
	}

	void StackAllocator::freeBytes(void* ptr, size_t size, size_t align)
	{
		//ASSERT(size != 0, "StackAllocator::freeBytes size parameter must not be zero."
		//		" To be more memory efficient, the allocated object array sizes are not stored."
		//		"Maybe you should use the function StackAllocator::free(T* ptr, size_t size)");
		//ASSERT(align != 0, "StackAllocator::freeBytes align parameter must not be zero (aligned stack)");
		// Free only if object is at the top of the stack
		if(reinterpret_cast<uint8_t*>(ptr)+size == &_memory[_current])
			_current -= size;
	}
}
