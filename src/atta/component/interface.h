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
#include <atta/component/entity.h>
#include <atta/component/factory.h>
#include <atta/component/manager.h>
#include <atta/component/registry/typedRegistry.h>

namespace atta::component {

class Factory;
class Entity;

void startUp();
void shutDown();

// Entity
Entity createEntity(EntityId entity = -1); // Can *try* to create entity with specific EntityId
void destroyEntity(Entity entity);
Entity copyEntity(Entity entity);

// Component
template <typename T>
inline ComponentId getId() {
    return TypedRegistry<T>::id;
}
std::vector<Registry*> getComponentRegistries();

// Factory
std::vector<Factory>& getFactories();
Factory* getFactory(Entity prototype);

// Views
std::vector<Entity> getEntitiesView();
std::vector<Entity> getNoPrototypeView();
std::vector<Entity> getScriptView();
Entity getSelectedEntity();
void setSelectedEntity(Entity entity);

// Memory management
void createDefault();

/**
 * @brief Clear all entities and components
 */
void clear();

} // namespace atta::component

#endif // ATTA_COMPONENT_INTERFACE_H
