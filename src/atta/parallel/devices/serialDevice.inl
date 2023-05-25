//--------------------------------------------------
// Atta Parallel Module
// serialDevice.inl
// Date: 2023-05-24
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::parallel {

template <typename Script>
void SerialDevice::run(cmp::Entity entity, float dt, uint32_t num) {
    cmp::EntityId start = entity.getId();
    cmp::EntityId end = entity.getId() + num;
    for (cmp::EntityId i = start; i < end; i++) {
        Script script;
        script.entity = cmp::Entity(i);
        script.dt = dt;
        script.update();
    }
}

} // namespace atta::parallel
