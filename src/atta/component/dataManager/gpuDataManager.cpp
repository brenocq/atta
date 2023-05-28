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

ATTA_GPU GpuDataManager* gpuDataManager;

ATTA_GPU void GpuDataManager::init() { gpuDataManager = (GpuDataManager*)cuda::alloc(sizeof(GpuDataManager)); }

ATTA_GPU void GpuDataManager::deinit() { cuda::free(gpuDataManager); }

} // namespace atta::component
