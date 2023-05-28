//--------------------------------------------------
// Atta Component Module
// cpuDataManager.cpp
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/dataManager/cpuDataManager.h>
#include <atta/component/registry/registry.h>

namespace atta::component {

CpuDataManager* cpuDataManager;

void CpuDataManager::init() {
    cpuDataManager = (CpuDataManager*)memory::getAllocator(SSID("ComponentAllocator"))->allocBytes(sizeof(CpuDataManager), sizeof(CpuDataManager));
    cpuDataManager->initMemory();
}

void CpuDataManager::deinit() {
    memory::getAllocator(SSID("ComponentAllocator"))->freeBytes(cpuDataManager, sizeof(CpuDataManager), sizeof(CpuDataManager));
}

void CpuDataManager::allocPool(ComponentId cid, uint32_t numComponents) {
    if (_componentPools[cid].isAllocated()) {
        LOG_ERROR("cmp::CpuDataManager", "Trying to add pool that is already allocated");
        return;
    }

    // Allocate pool
    uint32_t componentSize = Registry::get(cid)->getSizeof();
    uint32_t poolSize = ComponentPool::calcPoolSize(numComponents, componentSize);
    uint8_t* memory = (uint8_t*)_allocator->allocBytes(poolSize, componentSize);

    // Add pool
    _componentPools[cid] = ComponentPool(memory, poolSize, componentSize, numComponents);
}

void CpuDataManager::deallocPool(ComponentId cid) {
    if (!_componentPools[cid].isAllocated()) {
        LOG_ERROR("cmp::CpuDataManager", "Trying to removed pool that is not allocated");
        return;
    }

    // Deallocate pool
    _allocator->freeBytes(_componentPools[cid].getMemory(), _componentPools[cid].getSize(), _componentPools[cid].getComponentSize());

    // Remove pool
    _componentPools[cid].reset();
}

void CpuDataManager::copyCpuToGpu() {
    // uint8_t* memory = (uint8_t*)cuda::alloc(poolSize);
    // cuda::free(_componentPools[cid].getMemory());
}

void CpuDataManager::copyGpuToCpu() {}

void CpuDataManager::initMemory() {
    // Get allocator to allocate pools from
    _allocator = memory::getAllocator(SSID("ComponentAllocator"));

    // Initialize entity blocks
    initEntityPool();
}

} // namespace atta::component
