//--------------------------------------------------
// Atta Component System
// componentManager.h
// Date: 2021-11-09
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    template <typename T>
    T* ComponentManager::getEntityComponentImpl(EntityId entity)
    {
        return reinterpret_cast<T*>(getEntityComponentByIdImpl(COMPONENT_POOL_SSID(T), entity));
    }

    template <typename T>
    T* ComponentManager::addEntityComponentImpl(EntityId entity)
    {
        return reinterpret_cast<T*>(addEntityComponentByIdImpl(COMPONENT_POOL_SSID(T), entity));
    }
}
