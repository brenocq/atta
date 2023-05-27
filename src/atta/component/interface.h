//--------------------------------------------------
// Atta Component Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_INTERFACE_H
#define ATTA_COMPONENT_INTERFACE_H

#include <atta/component/base.h>
#include <atta/component/components/component.h>
#include <atta/component/registry/typedRegistry.h>

namespace atta::component {

class Factory;
class Entity;

void startUp();
void shutDown();

// Create/destroy entity
Entity createEntity(EntityId entity = -1); // Can *try* to create entity with specific EntityId
void deleteEntity(Entity entity);          // Delete entity and deallocate components
Entity copyEntity(Entity entity);

// Add component
template <typename T>
T* addComponent(Entity entity);
Component* addComponentById(ComponentId id, Entity entity);
Component* addComponentPtr(Entity entity, unsigned index, Component* component);

// Remove component
void removeComponentById(ComponentId id, Entity entity);

// Get component
template <typename T>
T* getComponent(Entity entity);
Component* getComponentById(ComponentId id, Entity entity);
std::vector<Component*> getComponents(Entity entity);

// Getters
std::vector<Registry*> getComponentRegistries();
std::vector<Factory>& getFactories();
Factory* getFactory(Entity prototype);

// Views
std::vector<EntityId> getEntitiesView();
std::vector<EntityId> getNoPrototypeView();
std::vector<EntityId> getCloneView();
std::vector<EntityId> getNoCloneView();
std::vector<EntityId> getScriptView();
Entity getSelectedEntity();
void setSelectedEntity(Entity entity);

// Memory management
void createDefault();
void clear();
void unregisterCustomComponents();

} // namespace atta::component

// Template definitions
#include <atta/component/entity.h>
#include <atta/component/manager.h>

namespace atta::component {

template <typename T>
ComponentId getId() {
    return COMPONENT_POOL_SID_BY_NAME(typeid(T).name());
}

template <typename T>
T* addComponent(Entity entity) {
    return Manager::getInstance().addComponentImpl<T>(entity);
}

template <typename T>
T* getComponent(Entity entity) {
    return Manager::getInstance().getComponentImpl<T>(entity);
}

} // namespace atta::component

#endif // ATTA_COMPONENT_INTERFACE_H
