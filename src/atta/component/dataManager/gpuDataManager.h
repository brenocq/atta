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
     * @brief Allocate resources used by GPU data manager
     */
    static ATTA_CPU void init();

    /**
     * @brief Release resources used by GPU data manager
     */
    static ATTA_CPU void deinit();


    /**
     * @brief Allocate GPU pool
     *
     * @param cid Component id
     * @param numComponents Number of components in the pool
     */
    static ATTA_CPU void allocPool(ComponentId cid, uint32_t numComponents);

    /**
     * @brief Deallocate GPU pool
     *
     * @param cid Component id
     */
    static ATTA_CPU void deallocPool(ComponentId cid);
};

extern ATTA_GPU GpuDataManager gpuDataManager;

} // namespace atta::component

#endif // ATTA_COMPONENT_DATA_MANAGER_GPU_DATA_MANAGER_H
