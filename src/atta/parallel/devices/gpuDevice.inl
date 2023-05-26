//--------------------------------------------------
// Atta Parallel Module
// gpuDevice.inl
// Date: 2023-05-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#if ATTA_IS_GPU_CODE
//#include <atta/script/scripts.h>
// #include <atta/script/scripts/controller.h>
// #include "/home/breno/Github/brenocq-atta/ants/src/ant.h"
#endif

namespace atta::parallel {

#if ATTA_IS_GPU_CODE
template <typename Script>
__global__ void kernel(cmp::EntityId firstClone, cmp::EntityId lastClone, float dt) {
    cmp::EntityId clone = firstClone + (blockIdx.x * blockDim.x + threadIdx.x);
    if (clone > lastClone)
        return;

    Script script;
    script.entity = clone;
    script.dt = dt;
    script.update();
}
#endif

template <typename Script>
void GpuDevice::run(cmp::Entity entity, float dt, uint32_t num) {
#if ATTA_IS_GPU_CODE
    constexpr uint32_t t = 256; // Number of threads in each block
    cmp::EntityId firstClone = entity.getId();
    cmp::EntityId lastClone = firstClone + num;
    kernel<Script><<<(num + t) / t, t>>>(firstClone, lastClone, dt);
#endif
}

} // namespace atta::parallel
