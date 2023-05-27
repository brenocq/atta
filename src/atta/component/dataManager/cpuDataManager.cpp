//--------------------------------------------------
// Atta Component Module
// cpuDataManager.cpp
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/dataManager/cpuDataManager.h>

namespace atta::component {

ATTA_CPU void CpuDataManager::init() {
    _allocator = memory::getAllocator(SSID("ComponentAllocator"));
    if (_allocator == nullptr)
        LOG_ERROR("cmp::CpuDataManager", "Could not find ComponentAllocator");

    // Initialize entity blocks
    initEntityPool();
}

ATTA_CPU void allocPool(ComponentId cid, uint32_t numComponents) {
    if (_componentPools[cid].isAllocated()) {
        LOG_ERROR("cmp::CpuDataManager", "Trying to add pool that is already allocated");
        return;
    }

    // Allocate pool
    uint32_t componentSize = Registry::get(cid)->getSizeof();
    uint32_t poolSize = ComponentPool::calcPoolSize(numComponents, componentSize);
    uint8_t* memory = _allocator->allocBytes(poolSize, componentSize);

    // Add pool
    _componentPools[cid] = ComponentPool(memory, poolSize, componentSize, numComponents);
}

ATTA_CPU void deallocPool(ComponentId cid) {
    if (!_componentPools[cid].isAllocated()) {
        LOG_ERROR("cmp::CpuDataManager", "Trying to removed pool that is not allocated");
        return;
    }

    // Deallocate pool
    _allocator->freeBytes(_componentPools[cid].memory, _componentPools[cid].size, _componentPools[cid].componentSize);

    // Remove pool
    _componentPools[cid].reset();
}

} // namespace atta::component
