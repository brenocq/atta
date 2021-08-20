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
}
