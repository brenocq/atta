//--------------------------------------------------
// Atta Component Module
// componentPool.h
// Date: 2023-05-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/dataManager/componentPool.h>

namespace atta::component {

ComponentPool::ComponentPool(uint8_t* memory, uint8_t size, uint32_t componentSize, uint32_t numComponents)
    : _memory(memory), _size(size), _componentSize(componentSize), _numComponents(numComponents), _current(0) {
    _dataStart = _memory + calcBitmapSize(_numComponents);
}

ATTA_CPU uint32_t ComponentPool::calcPoolSize(uint32_t numComponents, uint32_t componentSize) {
    return calcBitmapSize(numComponents) + componentSize * numComponents;
}

ATTA_CPU uint32_t ComponentPool::calcBitmapSize(uint32_t numComponents) {
    uint32_t size = ceil(numComponents / 8.0f);

    // Align size with 8 bytes
    constexpr uint32_t align = 8;
    return (size + (align - 1)) & ~(align - 1);
}

ATTA_CPU void ComponentPool::reset() {
    _memory = nullptr;
    _dataStart = nullptr;
    _size = 0;
    _componentSize = 0;
    _numComponents = 0;
    _current = 0;
}

ATTA_CPU_GPU Component* ComponentPool::alloc() {
    uint32_t start = _current;
    do {
        if (!getBit(_current)) {
            setBit(_current, true);
            Component* c = (Component*)getBlock(_current);
            _current = (_current + 1) % _numComponents;
            return c;
        }
        _current = (_current + 1) % _numComponents;
    } while (_current != start);
    return nullptr;
}

ATTA_CPU_GPU void ComponentPool::free(Component* ptr) {
    uint32_t index = getIndex(ptr);

    // Move allocation pointer
    _current = std::min(_current, index);

    setBit(index, false);
}

ATTA_CPU bool ComponentPool::isAllocated() const { return _memory != nullptr; }

ATTA_CPU_GPU uint8_t* ComponentPool::getMemory() const { return _memory; }

ATTA_CPU_GPU uint32_t ComponentPool::getSize() const { return _size; }

ATTA_CPU_GPU uint32_t ComponentPool::getComponentSize() const { return _componentSize; }

ATTA_CPU_GPU uint32_t ComponentPool::getNumComponents() const { return _numComponents; }

ATTA_CPU_GPU bool ComponentPool::getBit(uint64_t index) { return (_memory[index / 8] & (1 << (index % 8))) > 0; }

ATTA_CPU_GPU void ComponentPool::setBit(uint64_t index, bool bit) {
    _memory[index / 8] = (_memory[index / 8] & ~(1 << (index % 8))) // Clear bit
                         + (1 << (index % 8)) * bit;                // Set bit
}

ATTA_CPU_GPU uint32_t ComponentPool::getIndex(void* component) { return ((uint8_t*)(component)-_dataStart) / _componentSize; }

ATTA_CPU_GPU void* ComponentPool::getBlock(uint32_t index) { return getBit(index) ? (void*)(_dataStart + _componentSize * index) : nullptr; }

} // namespace atta::component
