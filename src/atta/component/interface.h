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
#include <atta/component/typedComponentRegistry.h>

namespace atta::component {

class Factory;
class Entity;

void startUp();
void shutDown();

// Create/destroy entity
Entity createEntity(EntityId entity = -1); // Can *try* to create entity with specific EntityId
void deleteEntity(Entity entity);          // Delete entity and deallocate components
void deleteEntityOnly(Entity entity);      // Delete entity without deallocating components
Entity copyEntity(Entity entity);

// Add component
template <typename T>
T* addComponent(Entity entity);
Component* addComponentById(ComponentId id, Entity entity);
Component* addComponentPtr(Entity entity, unsigned index, uint8_t* component);

// Get component
template <typename T>
T* getComponent(Entity entity);
Component* getComponentById(ComponentId id, Entity entity);
std::vector<Component*> getComponents(Entity entity);

// Remove component
void removeComponentById(ComponentId id, Entity entity);

// Getters
std::vector<ComponentRegistry*> getComponentRegistries();
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
void registerComponent(ComponentRegistry* componentRegistry);
void unregisterCustomComponents();

} // namespace atta::component

// Template definitions
#include <atta/component/entity.h>
#include <atta/component/manager.h>

namespace atta::component {

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
