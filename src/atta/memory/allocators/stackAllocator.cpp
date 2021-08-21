//--------------------------------------------------
// Atta Memory
// stackAllocator.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/allocators/stackAllocator.h>

namespace atta
{
	StackAllocator::StackAllocator(size_t size):
		_current(0), _size(size)
	{
		_memory = static_cast<uint8_t*>(malloc(size));
		//LOG_VERBOSE("StackAllocator", "Allocate heap memory of size $0", size);
	}

	StackAllocator::~StackAllocator()
	{
		free(_memory);
		//LOG_VERBOSE("StackAllocator", "Free heap memory");
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
