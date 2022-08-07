//--------------------------------------------------
// Atta Component Module
// componentManager.h
// Date: 2021-11-09
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    template <typename T>
    T* ComponentManager::getEntityComponentImpl(EntityId entity)
    {
		static int index = -1;
		if(index == -1)
			index = TypedComponentRegistry<T>::getInstance().getIndex();

        return reinterpret_cast<T*>(getEntityComponentByIndex(index, entity));
    }

    template <typename T>
    T* ComponentManager::addEntityComponentImpl(EntityId entity)
    {
        return reinterpret_cast<T*>(addEntityComponentByIdImpl(COMPONENT_POOL_SSID(T), entity));
    }
}
