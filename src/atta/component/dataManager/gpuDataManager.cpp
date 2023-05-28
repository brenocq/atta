//--------------------------------------------------
// Atta Component Module
// gpuDataManager.cpp
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/dataManager/gpuDataManager.h>
#include <atta/component/registry/registry.h>
#include <atta/utils/cuda.h>

namespace atta::component {

ATTA_CPU void GpuDataManager::init() {
    // Create gpu instance
    _gpuInstance = (GpuDataManager*)cuda::alloc(sizeof(GpuDataManager));

    // Initialize entity blocks
    initEntityPool();
}

ATTA_CPU void GpuDataManager::deinit() { cuda::free(_gpuInstance); }

ATTA_CPU void GpuDataManager::allocPool(ComponentId cid, uint32_t numComponents) {
    if (_componentPools[cid].isAllocated()) {
        LOG_ERROR("cmp::GpuDataManager", "Trying to add pool that is already allocated");
        return;
    }

    // Allocate pool
    uint32_t componentSize = Registry::get(cid)->getSizeof();
    uint32_t poolSize = ComponentPool::calcPoolSize(numComponents, componentSize);
    uint8_t* memory = (uint8_t*)cuda::alloc(poolSize);

    // Add pool
    _componentPools[cid] = ComponentPool(memory, poolSize, componentSize, numComponents);
}

ATTA_CPU void GpuDataManager::deallocPool(ComponentId cid) {
    if (!_componentPools[cid].isAllocated()) {
        LOG_ERROR("cmp::GpuDataManager", "Trying to removed pool that is not allocated");
        return;
    }

    // Deallocate pool
    cuda::free(_componentPools[cid].memory);

    // Remove pool
    _componentPools[cid].reset();
}

ATTA_CPU void GpuDataManager::copyCpuToGpu() {
    // TODO
}

ATTA_CPU void GpuDataManager::copyGpuToCpu() {
    // TODO
}

} // namespace atta::component
