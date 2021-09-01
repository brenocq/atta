//--------------------------------------------------
// Atta Memory System
// allocator.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memorySystem/allocator.h>

namespace atta
{
	Allocator::Allocator(uint64_t size)
	{
		LOG_DEBUG("Allocator", "Allocating $0GB", size/1024/1024/1024);
		_memory = new uint8_t[size];
		ASSERT(_memory != nullptr, "Could not allocate enough memory");

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
		LOG_DEBUG("Allocator", "Free $0GB", _size/1024/1024/1024);
		if(_shouldFree)
			delete _memory;
	}
}
