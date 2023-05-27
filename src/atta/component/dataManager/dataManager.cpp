//--------------------------------------------------
// Atta Component Module
// dataManager.cpp
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/dataManager/dataManager.h>

namespace atta::component {

ATTA_CPU_GPU uint8_t* DataManager::addComponent(Entity entity, ComponentId cid) { return nullptr; }

ATTA_CPU_GPU void DataManager::removeComponent(Entity entity, ComponentId cid) {}

ATTA_CPU_GPU uint8_t* DataManager::getComponent(Entity entity, ComponentId cid) { return nullptr; }

ATTA_CPU_GPU Entity DataManager::createEntity(EntityId entity) { return Entity{}; }

ATTA_CPU_GPU void DataManager::destroyEntity(Entity entity) {}

DataManager::ComponentPool::ComponentPool(uint8_t memory_, uint8_t size_, uint32_t componentSize_, uint32_t numComponents_)
    : memory(memory_), size(size_), bitmapSize(calcBitmapSize(numComponents, componentSize)), componentSize(componentSize_), current(0) {}

ATTA_CPU bool DataManager::ComponentPool::isAllocated() const { return _memory != nullptr; }

ATTA_CPU uint32_t DataManager::ComponentPool::calcPoolSize(uint32_t numComponents, uint32_t componentSize) {
    return calcBitmapSize(numComponents + componentSize) + componentSize * numComponents;
}

ATTA_CPU uint32_t DataManager::ComponentPool::calcBitmapSize(uint32_t numComponents, uint32_t componentSize) {
    // TODO take alignment into consideration
    return ceil(numComponents / 8.0f);
}

ATTA_CPU void reset() {
    memory = nullptr;
    size = 0;
    bitmapSize = 0;
    componentSize = 0;
    current = 0;
}

ATTA_CPU_GPU uint8_t* DataManager::ComponentPool::alloc() {}

ATTA_CPU_GPU void DataManager::ComponentPool::free(uint8_t* ptr) {}

ATTA_CPU void DataManager::initEntityPool() {
    for (EntityBlock& eb : _entityPool) {
        eb.exists = false;
        for (size_t i = 0; i < eb.components.size(); i++)
            eb.components[i] = nullptr;
    }
}

} // namespace atta::component
