//--------------------------------------------------
// Atta Component Module
// entity.inl
// Date: 2023-05-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#if ATTA_CPU_CODE
#include <atta/component/dataManager/cpuDataManager.h>
#include <atta/event/events/createComponent.h>
#include <atta/event/events/deleteComponent.h>
#include <atta/event/interface.h>
#else
// #include <atta/component/dataManager/gpuDataManager.h>
#endif
#include <atta/component/registry/typedRegistry.h>

namespace atta::component {

template <typename T>
ATTA_CPU_GPU T* Entity::add() const {

#if ATTA_GPU_CODE
    // return gpuDataManager->addComponent(_id, cid);
    return nullptr;
#else
    ComponentId cid = TypedRegistry<T>::id;
    T* component = (T*)cpuDataManager->addComponent(_id, cid);
    event::CreateComponent event;
    event.componentId = cid;
    event.entityId = _id;
    event.component = component;
    event::publish(event);
    return component;
#endif
}

template <typename T>
ATTA_CPU_GPU void Entity::remove() const {

#if ATTA_GPU_CODE
    // gpuDataManager->removeComponent(_id, cid);
#else
    ComponentId cid = TypedRegistry<T>::id;
    cpuDataManager->removeComponent(_id, cid);
    event::DeleteComponent event;
    event.componentId = cid;
    event.entityId = _id;
    event::publish(event);
#endif
}

template <typename T>
ATTA_CPU_GPU T* Entity::get() const {
#if ATTA_GPU_CODE
    // return (T*)gpuDataManager->getComponent(_id, cid);
    return nullptr;
#else
    ComponentId cid = TypedRegistry<T>::id;
    return (T*)cpuDataManager->getComponent(_id, cid);
#endif
}

} // namespace atta::component
