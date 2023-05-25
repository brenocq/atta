//--------------------------------------------------
// Atta Parallel Module
// interface.inl
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::parallel {

template <typename Script>
void run(cmp::Entity entity, float dt, uint32_t num) {
    Manager::getInstance().runImpl<Script>(entity, dt, num);
}

} // namespace atta::parallel
