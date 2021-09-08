//--------------------------------------------------
// Atta Memory System
// bitmapAllocator.cpp
// Date: 2021-08-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memorySystem/allocators/bitmapAllocator.h>

namespace atta
{
	BitmapAllocator::BitmapAllocator(uint64_t size):
		Allocator(size), _current(0)
	{
		init();
	}

	BitmapAllocator::BitmapAllocator(uint8_t* memory, uint64_t size):
		Allocator(memory, size), _current(0)
	{
		init();
	}

	void BitmapAllocator::init()
	{
		// x := Number of bytes in the bitmap
		// x + x*8 = size
		// 9*x = size
		// x = ceil(size/9)

		size_t bitmapSize = ceil(_size/9.0f);
		_dataStart = _memory + bitmapSize;
		_dataSize = _size - bitmapSize;

		for(size_t i = 0; i < bitmapSize; i++)
			_memory[i] = 0;
	}

	void* BitmapAllocator::allocBytes(size_t size, size_t align)
	{
		DASSERT(_size > 1, "Memory size must be greater than 1");

		const size_t start = _current;
		int firstFreeCount = -1;

		size_t curr = start;
		unsigned freeCount = 0;

		do {
			bool freeSpace = (_memory[curr/8] & (1 << (curr%8))) == 0;
			if(freeSpace)
				freeCount++;
			else
			{
				// Restart counter
				if(firstFreeCount == -1)
					firstFreeCount = freeCount;
				freeCount = 0;
			}

			// Check can allocate
			if(freeCount == size)
			{
				// Found free bytes
				// Set bits as allocated
				for(size_t i = curr-size+1; i <= curr; i++)
					_memory[i/8] = _memory[i/8] | (1 << (i%8));

				// Update _current for next search
				_current = (curr+1)%_dataSize;

				// Return pointer to first position
				return &_dataStart[curr-size+1];
			}

			curr++;
			// Check if need to return to first data byte
			if(curr >= _dataSize)
			{
				// Restart counter
				if(firstFreeCount == -1)
					firstFreeCount = freeCount;
				freeCount = 0;

				// Return to first data byte
				curr = 0;
			}
		} while(curr != start);

		if(firstFreeCount+freeCount >= size)
		{
			// Found free bytes (bytes before start + bytes after start)
			// Set bits as allocated
			for(size_t i = curr-freeCount+1; i <= curr; i++)
				_memory[i/8] = _memory[i/8] | (1 << (i%8));

			// Update _current for next search
			_current = (curr-freeCount+size+1)%_dataSize;

			// Return pointer to first position
			return &_dataStart[curr-freeCount+1];
		}

		return nullptr;
	}

	void BitmapAllocator::freeBytes(void* ptr, size_t size, size_t align)
	{
		size_t start = static_cast<uint8_t*>(ptr) - _dataStart;

		// XXX Can be faster if set byte to zero
		for(size_t i = start; i < start+size; i++)
			_memory[i/8] = _memory[i/8] & ~(1 << (i%8));
	}
}
