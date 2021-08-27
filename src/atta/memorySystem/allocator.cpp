//--------------------------------------------------
// Atta Memory System
// allocator.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memorySystem/allocator.h>

namespace atta
{
	Allocator::Allocator(size_t size)
	{
		_memory = static_cast<uint8_t*>(malloc(size));
		_size = size;
		_shouldFree = true;
	}

	Allocator::Allocator(uint8_t* memory, size_t size)
	{
		_memory = memory;
		_size = size;
		_shouldFree = false;
	}

	Allocator::~Allocator()
	{
		if(_shouldFree)
			delete _memory;
	}
}
