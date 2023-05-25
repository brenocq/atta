//--------------------------------------------------
// Atta Parallel Module
// gpuDevice.inl
// Date: 2023-05-24
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::parallel {

template <typename Script>
void GpuDevice::run(cmp::Entity entity, float dt, uint32_t num) {
    Script script;
    cmp::EntityId eidFirst = entity.getId();
    for (cmp::EntityId eid = eidFirst; eid < eidFirst + num; eid++)
        script.update(cmp::Entity(eid), dt);
}

} // namespace atta::parallel
