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
Entity createEntity(EntityId entity) { return Manager::getInstance().createEntityImpl(entity); }
void deleteEntity(Entity entity) { return Manager::getInstance().deleteEntityImpl(entity); }
Entity copyEntity(Entity entity) { return Manager::getInstance().copyEntityImpl(entity); }

// Add entity component
Component* addComponentById(ComponentId id, Entity entity) { return Manager::getInstance().addComponentByIdImpl(id, entity); }
Component* addComponentPtr(Entity entity, unsigned index, Component* component) {
    return Manager::getInstance().addComponentPtrImpl(entity, index, component);
}
// Get entity component
Component* getComponentById(ComponentId id, Entity entity) { return Manager::getInstance().getComponentByIdImpl(id, entity); }
std::vector<Component*> getComponents(Entity entity) { return Manager::getInstance().getComponentsImpl(entity); }
// Remove entity component
void removeComponentById(ComponentId id, Entity entity) { Manager::getInstance().removeComponentByIdImpl(id, entity); }

// Getters
std::vector<Registry*> getComponentRegistries() { return Manager::getInstance().getComponentRegistriesImpl(); }
std::vector<Factory>& getFactories() { return Manager::getInstance().getFactoriesImpl(); }
Factory* getFactory(Entity prototype) { return Manager::getInstance().getFactoryImpl(prototype); }

// Views
std::vector<EntityId> getEntitiesView() { return Manager::getInstance().getEntitiesViewImpl(); }
std::vector<EntityId> getNoPrototypeView() { return Manager::getInstance().getNoPrototypeViewImpl(); }
std::vector<EntityId> getCloneView() { return Manager::getInstance().getCloneViewImpl(); }
std::vector<EntityId> getNoCloneView() { return Manager::getInstance().getNoCloneViewImpl(); }
std::vector<EntityId> getScriptView() { return Manager::getInstance().getScriptViewImpl(); }
Entity getSelectedEntity() { return Manager::getInstance()._selectedEntity; }
void setSelectedEntity(Entity eid) { Manager::getInstance()._selectedEntity = eid; }

// Memory management
void createDefault() { Manager::getInstance().createDefaultImpl(); }
void clear() { Manager::getInstance().clearImpl(); }
void registerComponent(Registry* componentRegistry) { return Manager::getInstance().registerComponentImpl(componentRegistry); }
void unregisterCustomComponents() { Manager::getInstance().unregisterCustomComponentsImpl(); }

} // namespace atta::component
