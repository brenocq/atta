//--------------------------------------------------
// Atta Memory
// allocatedObject.h
// Date: 2021-08-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MEMORY_ALLOCATED_OBJECT_H
#define ATTA_MEMORY_ALLOCATED_OBJECT_H
#include <atta/core/stringId.h>

namespace atta
{
	// Objects that want to be allocated using atta allocators should derive from this class
	// and specify the allocator stringHash
	template <class Object, StringHash allocatorId>
	class AllocatedObject
	{
	public:
		// Throwing operator new
		void* operator new(size_t size);
		// Nothrow operator new
		void* operator new(std::size_t size, const std::nothrow_t& nothrow_value) noexcept;
		// Placement operator new
		void* operator new(size_t size, void* ptr) noexcept;

		// Throwing operator new[]
		void* operator new[](size_t size);
		// Nothrow operator new[]
		void* operator new[](std::size_t size, const std::nothrow_t& nothrow_value) noexcept;
		// Placement operator new[]
		void* operator new[](size_t size, void* ptr) noexcept;

		// Ordinary operator delete
		void operator delete(void* ptr) noexcept;
		// Nothrow operator delete
		void operator delete(void* ptr, const std::nothrow_t& nothrow_constant) noexcept;
		// Placement operator delete
		void operator delete(void* ptr, void* voidptr2) noexcept;

		// Ordinary operator delete[]
		void operator delete[](void* ptr) noexcept;
		// Nothrow operator delete[]
		void operator delete[](void* ptr, const std::nothrow_t& nothrow_constant) noexcept;
		// Placement operator delete[]
		void operator delete[](void* ptr, void* voidptr2) noexcept;

		// Ordinary operator delete[] with size
		void operator delete[](void* ptr, std::size_t size) noexcept;
		// Nothrow operator delete[] with size
		void operator delete[](void* ptr, std::size_t size, const std::nothrow_t& nothrow_constant) noexcept;

	private:
		static inline void* allocBytes(size_t size, size_t align);
		static inline void freeBytes(void* ptr, size_t size, size_t align);
	};
}

#include <atta/memory/allocatedObject.inl>
#endif// ATTA_MEMORY_ALLOCATED_OBJECT_H
