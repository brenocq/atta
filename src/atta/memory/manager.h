// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

namespace atta::memory {

class Manager final {
  public:
    static Manager& getInstance();

    friend void registerAllocator(StringHash hash, Allocator* alloc);
    template <typename T>
    friend T* getAllocator(StringHash hash);
    template <typename T>
    friend T** getAllocatorPtr(StringHash hash);

  private:
    Allocator* getAllocatorImpl(StringHash hash);
    Allocator** getAllocatorPtrImpl(StringHash hash);
    void registerAllocatorImpl(StringHash hash, Allocator* alloc);

    std::unordered_map<StringHash, Allocator*> _allocators;
};

} // namespace atta::memory
