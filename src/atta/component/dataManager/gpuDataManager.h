//--------------------------------------------------
// Atta Component Module
// gpuDataManager.h
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_DATA_MANAGER_GPU_DATA_MANAGER_H
#define ATTA_COMPONENT_DATA_MANAGER_GPU_DATA_MANAGER_H
#include <atta/component/dataManager/dataManager.h>

namespace atta::component {

class GpuDataManager : public DataManager {
  public:
    /**
     * @brief Create GPU instance
     */
    ATTA_CPU void init();

    /**
     * @brief Free GPU data
     */
    ATTA_CPU void deinit();

    /**
     * @brief Allocae pool
     *
     * @param cid Component id
     * @param numComponents Number of components in the pool
     */
    ATTA_CPU void allocPool(ComponentId cid, uint32_t numComponents);

    /**
     * @brief Deallocate pool
     *
     * @param cid Component id
     */
    ATTA_CPU void deallocPool(ComponentId cid);

    /**
     * @brief Copy pools from CPU to GPU
     */
    ATTA_CPU void copyCpuToGpu();

    /**
     * @brief Copy pools from GPU to CPU
     */
    ATTA_CPU void copyGpuToCpu();

  private:
    GpuDataManager* _gpuInstance;
};

} // namespace atta::component

#endif // ATTA_COMPONENT_DATA_MANAGER_GPU_DATA_MANAGER_H
