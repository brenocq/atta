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
      ATTA_CPU GpuDataManager();

  private:
      GpuDataManager* _gpuInstance;
};

} // namespace atta::component

#endif // ATTA_COMPONENT_DATA_MANAGER_GPU_DATA_MANAGER_H
