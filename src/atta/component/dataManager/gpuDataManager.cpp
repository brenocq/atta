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

GpuDataManager cpuCopy;
ATTA_GPU GpuDataManager gpuDataManager;

ATTA_CPU void updateGPU() { cudaMemcpyToSymbol(gpuDataManager, &cpuCopy, sizeof(GpuDataManager), 0, cudaMemcpyHostToDevice); }

ATTA_CPU void GpuDataManager::init() {
    cpuCopy = GpuDataManager();
    updateGPU();
}

ATTA_CPU void GpuDataManager::deinit() {
    for (ComponentPool& pool : cpuCopy._componentPools)
        if (pool.isAllocated())
            cuda::free(pool.getMemory());
}

ATTA_CPU void GpuDataManager::allocPool(ComponentId cid, uint32_t numComponents) {
    if (cpuCopy._componentPools[cid].isAllocated()) {
        LOG_ERROR("cmp::GpuDataManager", "Trying to add pool that is already allocated");
        return;
    }

    // Allocate pool
    uint32_t componentSize = Registry::get(cid)->getSizeof();
    uint32_t poolSize = ComponentPool::calcPoolSize(numComponents, componentSize);
    uint8_t* memory = (uint8_t*)cuda::alloc(poolSize);

    // Add pool
    cpuCopy._componentPools[cid] = ComponentPool(memory, poolSize, componentSize, numComponents);
    updateGPU();
}

ATTA_CPU void GpuDataManager::deallocPool(ComponentId cid) {
    if (!cpuCopy._componentPools[cid].isAllocated()) {
        LOG_ERROR("cmp::CpuDataManager", "Trying to removed pool that is not allocated");
        return;
    }

    // Deallocate pool
    cuda::free(cpuCopy._componentPools[cid].getMemory());

    // Remove pool
    cpuCopy._componentPools[cid].reset();
    updateGPU();
}

} // namespace atta::component
