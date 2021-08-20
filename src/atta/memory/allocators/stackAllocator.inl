//--------------------------------------------------
// Atta Memory
// stackAllocator.inl
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
	template<typename T>
	T* StackAllocator::allocate(size_t size)
	{
		if(_current + sizeof(T)*size > _size)
			return nullptr;
		else
		{
			T* ptr = reinterpret_cast<T*>(_memory + _current);
			_current += sizeof(T)*size;
			return ptr;
		}
	}

	template<typename T>
	void StackAllocator::free(T* object)
	{
		// Free only if object is at the top of the stack
		if(reinterpret_cast<uint8_t*>(object+1) == &_memory[_current])
			_current -= sizeof(T);
	}
}
