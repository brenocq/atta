//--------------------------------------------------
// Atta Parallel Module
// cpuDevice.inl
// Date: 2023-05-24
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::parallel {

template <typename Script>
void CpuDevice::run(cmp::Entity entity, float dt, uint32_t num) {
    cmp::EntityId start = entity.getId();
    cmp::EntityId end = entity.getId() + num - 1;
    run(start, end, [&](uint32_t i) {
        Script script;
        script.entity = cmp::Entity(i);
        script.dt = dt;
        script.update();
    });
}

} // namespace atta::parallel
