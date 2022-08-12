//--------------------------------------------------
// Atta Component Module
// manager.h
// Date: 2021-11-09
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::component {

template <typename T>
T* Manager::getEntityComponentImpl(EntityId entity) {
    static int index = -1;
    if (index == -1)
        index = TypedComponentRegistry<T>::getInstance().getIndex();

    return reinterpret_cast<T*>(getEntityComponentByIndex(index, entity));
}

template <typename T>
T* Manager::addEntityComponentImpl(EntityId entity) {
    return reinterpret_cast<T*>(addEntityComponentByIdImpl(COMPONENT_POOL_SSID(T), entity));
}

} // namespace atta::component
