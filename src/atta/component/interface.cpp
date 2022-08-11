//--------------------------------------------------
// Atta Component Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/interface.h>

namespace atta::component {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }

// Create/destroy entity
EntityId createEntity(EntityId entity) { return Manager::getInstance().createEntityImpl(entity); }
void deleteEntity(EntityId entity) { return Manager::getInstance().deleteEntityImpl(entity); }
void deleteEntityOnly(EntityId entity) { return Manager::getInstance().deleteEntityOnlyImpl(entity); }
EntityId copyEntity(EntityId entity) { return Manager::getInstance().copyEntityImpl(entity); }

// Add entity component
Component* addEntityComponentById(ComponentId id, EntityId entity) { return Manager::getInstance().addEntityComponentByIdImpl(id, entity); }
Component* addEntityComponentPtr(EntityId entity, unsigned index, uint8_t* component) {
    return Manager::getInstance().addEntityComponentPtrImpl(entity, index, component);
}
// Get entity component
Component* getEntityComponentById(ComponentId id, EntityId entity) { return Manager::getInstance().getEntityComponentByIdImpl(id, entity); }
std::vector<Component*> getEntityComponents(EntityId entity) { return Manager::getInstance().getEntityComponentsImpl(entity); }
// Remove entity component
void removeEntityComponentById(ComponentId id, EntityId entity) { Manager::getInstance().removeEntityComponentByIdImpl(id, entity); }

// Getters
std::vector<ComponentRegistry*> getComponentRegistries() { return Manager::getInstance().getComponentRegistriesImpl(); }
std::vector<Factory>& getFactories() { return Manager::getInstance().getFactoriesImpl(); }
Factory* getPrototypeFactory(EntityId prototype) { return Manager::getInstance().getPrototypeFactoryImpl(prototype); }

// Views
std::vector<EntityId> getEntitiesView() { return Manager::getInstance().getEntitiesViewImpl(); }
std::vector<EntityId> getNoPrototypeView() { return Manager::getInstance().getNoPrototypeViewImpl(); }
std::vector<EntityId> getCloneView() { return Manager::getInstance().getCloneViewImpl(); }
std::vector<EntityId> getNoCloneView() { return Manager::getInstance().getNoCloneViewImpl(); }
std::vector<EntityId> getScriptView() { return Manager::getInstance().getScriptViewImpl(); }
EntityId getSelectedEntity() { return Manager::getInstance()._selectedEntity; }
void setSelectedEntity(EntityId eid) { Manager::getInstance()._selectedEntity = eid; }

// Memory management
void createDefault() { Manager::getInstance().createDefaultImpl(); }
void clear() { Manager::getInstance().clearImpl(); }
void registerComponent(ComponentRegistry* componentRegistry) { return Manager::getInstance().registerComponentImpl(componentRegistry); }
void unregisterCustomComponents() { Manager::getInstance().unregisterCustomComponentsImpl(); }

} // namespace atta::component
