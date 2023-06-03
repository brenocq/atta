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

class GpuDataManager;
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
     * @brief Allocate pool
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

  private:
    /**
     * @brief Initialize CPU memory
     */
    void initMemory();

    memory::Allocator* _allocator;
    friend GpuDataManager;
};

extern CpuDataManager* cpuDataManager;

} // namespace atta::component

#endif // ATTA_COMPONENT_DATA_MANAGER_CPU_DATA_MANAGER_H
