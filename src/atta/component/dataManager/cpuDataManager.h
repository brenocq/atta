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
     * @brief Initialize internal data
     */
    ATTA_CPU void init();

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

  private:
    memory::Allocator* _allocator;
};

} // namespace atta::component

#endif // ATTA_COMPONENT_DATA_MANAGER_CPU_DATA_MANAGER_H
