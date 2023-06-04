//--------------------------------------------------
// Atta Component Module
// gpuDataManager.cpp
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/dataManager/cpuDataManager.h>
#include <atta/component/dataManager/gpuDataManager.h>
#include <atta/component/registry/registry.h>
#include <atta/utils/cuda.h>
#include <cuda_runtime.h>

namespace atta::component {

GpuDataManager cpuCopy; ///< CPU copy of gpuDataManager
ATTA_GPU GpuDataManager gpuDataManager;

ATTA_CPU void updateGPU() { cudaMemcpyToSymbol(gpuDataManager, &cpuCopy, sizeof(GpuDataManager)); }

ATTA_CPU void updateCPU() { cudaMemcpyFromSymbol(&cpuCopy, gpuDataManager, sizeof(GpuDataManager)); }

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

ATTA_CPU void GpuDataManager::copyCpuToGpu() {
    //---------- Copy component pools ----------//
    // XXX Assuming both will have the same pools with the same size
    auto& pools = cpuDataManager->_componentPools;
    for (size_t i = 0; i < maxComponents; i++)
        if (pools[i].isAllocated())
            cuda::copyCpuToGpu(cpuCopy._componentPools[i].getMemory(), pools[i].getMemory(), pools[i].getSize());

    //---------- Copy entity pool ----------//
    for (size_t i = 0; i < maxEntities; i++)
        cpuCopy._entityPool[i] = cpuDataManager->_entityPool[i];

    // Shift entity pool pointer addresses
    for (size_t i = 0; i < maxComponents; i++) {
        int64_t shift = (int64_t)cpuCopy._componentPools[i].getMemory() - (int64_t)pools[i].getMemory();
        for (size_t e = 0; e < maxEntities; e++)
            if (cpuCopy._entityPool[e].exist && cpuCopy._entityPool[e].components[i]) {
                cpuCopy._entityPool[e].components[i] = (Component*)((uint8_t*)cpuCopy._entityPool[e].components[i] + shift);
            }
    }

    updateGPU();
}

ATTA_CPU void GpuDataManager::copyGpuToCpu() {
    //---------- Copy component pools ----------//
    // XXX Assuming both will have the same pools with the same size
    auto& pools = cpuDataManager->_componentPools;
    for (size_t i = 0; i < maxComponents; i++)
        if (pools[i].isAllocated()) {
            // LOG_DEBUG("GpuDataManager", "Copy $0 -> $1 bytes", i, pools[i].getSize());
            cuda::copyGpuToCpu(pools[i].getMemory(), cpuCopy._componentPools[i].getMemory(), pools[i].getSize());
        }

    // Copy entity pool
    // TODO
}

} // namespace atta::component
