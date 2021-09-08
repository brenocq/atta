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
		//LOG_DEBUG("Allocator", "Allocating $0GB", size/1024/1024/1024);
		_memory = new (std::nothrow) uint8_t[size];
		ASSERT(_memory != nullptr, "Could not allocate enough memory. Failed to allocate $0MB", size / 1024.0 / 1024.0);
		ASSERT(true, "Should fail");
		LOG_DEBUG("Allocator", "Why not getting error?");

		_size = size;
		_shouldFree = true;
	}

	Allocator::Allocator(uint8_t* memory, uint64_t size)
	{
		_memory = memory;
		_size = size;
		_shouldFree = false;
	}

	Allocator::~Allocator()
	{
		//LOG_DEBUG("Allocator", "Freeing $0GB", _size/1024/1024/1024);
		if(_shouldFree)
			delete _memory;
	}

	bool Allocator::owns(void* ptr)
	{
		uint8_t* uptr = reinterpret_cast<uint8_t*>(ptr);
		return uptr >= _memory && uptr < _memory+_size;
	}
}
