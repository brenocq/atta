//--------------------------------------------------
// Atta Memory System
// poolAllocator.h
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H
#define ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H
#include <atta/memorySystem/allocator.h>

namespace atta
{
	template<typename T>
	class PoolAllocator final : public Allocator
	{
	public:
		// If blockAlign is set to zero, no alignment
		// Allocate heap memory
		PoolAllocator(size_t countBlocks, size_t blockAlign = 0);
		// Use already allocated memory
		PoolAllocator(uint8_t* memory, size_t countBlocks, size_t blockAlign = 0);

		// Simplified alloc/free
		T* alloc(size_t count = 1);
		void free(T* ptr, size_t count = 1);

		void* allocBytes(size_t size, size_t align) override;
		void freeBytes(void* ptr, size_t size, size_t align) override;

		void clear();
		uint64_t getIndex(T* block);
		T* getBlock(uint64_t index);// Return the block even if it is free, this can broke the pool allocator

	private:
		union Block
		{
			T object;
			Block* next;
		};

		size_t _blockCount;
		size_t _blockAlign;
		Block* _freeList;
	};
}

#include <atta/memorySystem/allocators/poolAllocator.inl>
#endif// ATTA_MEMORY_ALLOCATORS_POOL_ALLOCATOR_H
