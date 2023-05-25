//--------------------------------------------------
// Atta Parallel Module
// serialDevice.inl
// Date: 2023-05-24
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::parallel {

template <typename Script>
void SerialDevice::run(cmp::Entity entity, float dt, uint32_t num) {
    Script script;
    cmp::EntityId start = entity.getId();
    cmp::EntityId end = entity.getId() + num;
    for (cmp::EntityId i = start; i < end; i++)
        script.update(cmp::Entity(i), dt);
}

} // namespace atta::parallel
