//--------------------------------------------------
// Atta Memory Module
// allocator.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/allocator.h>

namespace atta::memory {

Allocator::Allocator(uint64_t size) {
    _memory = new (std::nothrow) uint8_t[size];
    ASSERT(_memory != nullptr, "Could not allocate enough memory. Failed to allocate $0MB", size / 1024.0 / 1024.0);

    _size = size;
    _shouldFree = true;
}

Allocator::Allocator(uint8_t* memory, uint64_t size) {
    _memory = memory;
    _size = size;
    _shouldFree = false;
}

Allocator::~Allocator() {
    // LOG_DEBUG("Allocator", "Freeing $0GB", _size/1024/1024/1024);
    if (_shouldFree)
        delete _memory;
}

bool Allocator::owns(void* ptr) {
    uint8_t* uptr = reinterpret_cast<uint8_t*>(ptr);
    return uptr >= _memory && uptr < _memory + _size;
}

} // namespace atta::memory
