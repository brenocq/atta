//--------------------------------------------------
// Atta Memory Module
// mallocAllocator.cpp
// Date: 2021-08-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/allocators/mallocAllocator.h>

namespace atta::memory {
MallocAllocator::MallocAllocator() : Allocator(nullptr, 0) {}

void* MallocAllocator::allocBytes(size_t size, size_t align) { return malloc(size); }

void MallocAllocator::freeBytes(void* ptr, size_t size, size_t align) { free(ptr); }
} // namespace atta::memory
