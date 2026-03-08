// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/memory/interface.h>

namespace atta::memory {

void registerAllocator(StringHash hash, Allocator* alloc) { Manager::getInstance().registerAllocatorImpl(hash, alloc); }

} // namespace atta::memory
