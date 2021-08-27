//--------------------------------------------------
// Atta Memory System
// poolAllocator.inl
// Date: 2021-08-25
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
	template <typename T>
	PoolAllocator<T>::PoolAllocator(size_t blockCount, size_t blockAlign):
		Allocator(blockAlign == 0 ? sizeof(Block)*blockCount : blockAlign*blockCount),
		_blockCount(blockCount),
		_blockAlign(blockAlign == 0 ? sizeof(Block) : blockAlign),
		_freeList(nullptr)
	{
		DASSERT(_blockCount > 0, "Pool should have more than 0 blocks");
		DASSERT(_blockAlign >= sizeof(Block), "Block align must be equal or greater than the block size");

		// Create free list
		clear();
	}

	template <typename T>
	PoolAllocator<T>::PoolAllocator(uint8_t* memory, size_t blockCount, size_t blockAlign):
		Allocator(memory, blockAlign == 0 ? sizeof(Block)*blockCount : blockAlign*blockCount),
		_blockCount(blockCount),
		_blockAlign(blockAlign == 0 ? sizeof(Block) : blockAlign),
		_freeList(nullptr)
	{
		DASSERT(_blockCount > 0, "Pool should have more than 0 blocks");
		DASSERT(_blockAlign >= sizeof(Block), "Block align must be equal or greater than the block size");

		// Create free list
		clear();
	}

	template <typename T>
	T* PoolAllocator<T>::alloc(size_t count)
	{
		DASSERT(count == 1, "Allocation with more than one block is not supported yet");
		return static_cast<T*>(allocBytes(sizeof(T)*count, sizeof(T)));
	}

	template <typename T>
	void PoolAllocator<T>::free(T* ptr, size_t count)
	{
		DASSERT(count == 1, "Free with more than one block is not supported yet");

		freeBytes(reinterpret_cast<void*>(ptr), sizeof(T), sizeof(T));
	}

	template <typename T>
	void PoolAllocator<T>::clear()
	{
		Block* block = reinterpret_cast<Block*>(Allocator::_memory);
		_freeList = block;
		for(size_t i = 0; i < _blockCount-1; i++)
		{
			block[i].next = &block[i+1];
		}
		block[_blockCount-1].next = nullptr;
	}

	template <typename T>
	void* PoolAllocator<T>::allocBytes(size_t size, size_t align)
	{
		DASSERT(size == sizeof(T), "AllocBytes with more than one block is not supported yet");

		// Check if there are block availables
		if(_freeList == nullptr)
			return nullptr;

		// Update freelist and return block
		Block* block = _freeList;
		_freeList = block->next;
		return reinterpret_cast<void*>(&block->object);
	}

	template <typename T>
	void PoolAllocator<T>::freeBytes(void* ptr, size_t size, size_t align)
	{
		DASSERT(size == sizeof(T), "AllocBytes with more than one block is not supported yet");

		Block* block = static_cast<Block*>(ptr);
		Block* last = _freeList;

		// Add block to end of the list
		block->next = last;
		// Update list root
		_freeList = block;
	}
}
