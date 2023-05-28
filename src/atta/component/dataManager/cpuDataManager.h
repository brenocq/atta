//--------------------------------------------------
// Atta Component Module
// cpuDataManager.h
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_DATA_MANAGER_CPU_DATA_MANAGER_H
#define ATTA_COMPONENT_DATA_MANAGER_CPU_DATA_MANAGER_H
#include <atta/component/dataManager/dataManager.h>
#include <atta/memory/interface.h>

namespace atta::component {

class CpuDataManager : public DataManager {
  public:
    /**
     * @brief Allocate resources used by CPU data manager
     */
    static void init();

    /**
     * @brief Release resources used by CPU data manager
     */
    static void deinit();

    /**
     * @brief Allocae pool
     *
     * @param cid Component id
     * @param numComponents Number of components in the pool
     */
    void allocPool(ComponentId cid, uint32_t numComponents);

    /**
     * @brief Deallocate pool
     *
     * @param cid Component id
     */
    void deallocPool(ComponentId cid);

    /**
     * @brief Copy pools from CPU to GPU
     */
    void copyCpuToGpu();

    /**
     * @brief Copy pools from GPU to CPU
     */
    void copyGpuToCpu();

  private:
    /**
     * @brief Initialize CPU memory
     */
    void initMemory();

    memory::Allocator* _allocator;
};

extern CpuDataManager* cpuDataManager;

} // namespace atta::component

#endif // ATTA_COMPONENT_DATA_MANAGER_CPU_DATA_MANAGER_H
