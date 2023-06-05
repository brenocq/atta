//--------------------------------------------------
// Atta Component Module
// entity.inl
// Date: 2023-05-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/dataManager/cpuDataManager.h>
#include <atta/component/registry/typedRegistry.h>
#include <atta/event/events/createComponent.h>
#include <atta/event/events/deleteComponent.h>
#include <atta/event/interface.h>

#include <atta/component/dataManager/gpuDataManager.h>

namespace atta::component {

template <typename T>
ATTA_CPU_GPU T* Entity::add() const {
#if ATTA_GPU_CODE
    ComponentId cid = idGpu<T>;
    T* component = (T*)gpuDataManager.addComponent(_id, cid);
    *component = T{};
    return component;
#else
    ComponentId cid = TypedRegistry<T>::id;
    T* component = (T*)cpuDataManager->addComponent(_id, cid);
    *component = T{};

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
    ComponentId cid = idGpu<T>;
    gpuDataManager.removeComponent(_id, cid);
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
    ComponentId cid = idGpu<T>;
    return (T*)gpuDataManager.getComponent(_id, cid);
#else
    ComponentId cid = TypedRegistry<T>::id;
    return (T*)cpuDataManager->getComponent(_id, cid);
#endif
}

} // namespace atta::component
