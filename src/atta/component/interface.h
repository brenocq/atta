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
#include <atta/component/factory.h>
#include <atta/component/typedComponentRegistry.h>

namespace atta::component {

void startUp();
void shutDown();

// Create/destroy entity
EntityId createEntity(EntityId entity = -1); // Can *try* to create entity with specific EntityId
void deleteEntity(EntityId entity);          // Delete entity and deallocate components
void deleteEntityOnly(EntityId entity);      // Delete entity without deallocating components
EntityId copyEntity(EntityId entity);

// Add entity component
template <typename T>
T* addEntityComponent(EntityId entity);
Component* addEntityComponentById(ComponentId id, EntityId entity);
Component* addEntityComponentPtr(EntityId entity, unsigned index, uint8_t* component);

// Get entity component
template <typename T>
T* getEntityComponent(EntityId entity);
Component* getEntityComponentById(ComponentId id, EntityId entity);
std::vector<Component*> getEntityComponents(EntityId entity);

// Remove entity component
void removeEntityComponentById(ComponentId id, EntityId entity);

// Getters
std::vector<ComponentRegistry*> getComponentRegistries();
std::vector<Factory>& getFactories();
Factory* getPrototypeFactory(EntityId prototype);

// Views
std::vector<EntityId> getEntitiesView();
std::vector<EntityId> getNoPrototypeView();
std::vector<EntityId> getCloneView();
std::vector<EntityId> getNoCloneView();
std::vector<EntityId> getScriptView();
EntityId getSelectedEntity();
void setSelectedEntity(EntityId eid);

// Memory management
void createDefault();
void clear();
void registerComponent(ComponentRegistry* componentRegistry);
void unregisterCustomComponents();

} // namespace atta::component

// Template definitions
#include <atta/component/manager.h>

namespace atta::component {

template <typename T>
T* addEntityComponent(EntityId entity) {
    return Manager::getInstance().addEntityComponentImpl<T>(entity);
}

template <typename T>
T* getEntityComponent(EntityId entity) {
    return Manager::getInstance().getEntityComponentImpl<T>(entity);
}

} // namespace atta::component

#endif // ATTA_COMPONENT_INTERFACE_H
