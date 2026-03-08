// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/memory/allocators/mallocAllocator.h>

namespace atta::memory {

MallocAllocator::MallocAllocator() : Allocator(nullptr, 0) {}

void* MallocAllocator::allocBytes(size_t size, size_t align) { return malloc(size); }

void MallocAllocator::freeBytes(void* ptr, size_t size, size_t align) { free(ptr); }

} // namespace atta::memory
