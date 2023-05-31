//--------------------------------------------------
// Atta Parallel Module
// gpuDevice.inl
// Date: 2023-05-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/dataManager/gpuDataManager.h>

namespace atta::parallel {

//#if defined(__NVCC__)
//template <typename Script>
//__global__ void kernel(cmp::EntityId firstClone, cmp::EntityId lastClone, float dt) {
//    cmp::EntityId clone = firstClone + (blockIdx.x * blockDim.x + threadIdx.x);
//    if (clone > lastClone)
//        return;
//
//    Script script;
//    script.entity = clone;
//    script.dt = dt;
//    script.update();
//}
//#endif

template <typename Script>
void GpuDevice::run(cmp::Entity entity, float dt, uint32_t num) {
//#if defined(__NVCC__)
//    constexpr uint32_t t = 256; // Number of threads in each block
//    cmp::EntityId firstClone = entity.getId();
//    cmp::EntityId lastClone = firstClone + num;
//    kernel<Script><<<(num + t) / t, t>>>(firstClone, lastClone, dt);
//#endif
}

} // namespace atta::parallel
