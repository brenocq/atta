//--------------------------------------------------
// Atta Component Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/interface.h>

namespace atta::component {

static EntityId _selectedEntity = -1; // TODO move to another place

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }

// Create/destroy entity
Entity createEntity(EntityId entity) { return Manager::getInstance().createEntityImpl(entity); }
void destroyEntity(Entity entity) { return Manager::getInstance().destroyEntityImpl(entity); }
Entity copyEntity(Entity entity) { return Manager::getInstance().copyEntityImpl(entity); }

// Getters
std::vector<Registry*> getComponentRegistries() { return Registry::get(); }
std::vector<Factory>& getFactories() { return Manager::getInstance().getFactoriesImpl(); }
Factory* getFactory(Entity prototype) { return Manager::getInstance().getFactoryImpl(prototype); }

// Views
std::vector<Entity> getEntitiesView() { return Manager::getInstance().getEntitiesViewImpl(); }
std::vector<Entity> getNoPrototypeView() { return Manager::getInstance().getNoPrototypeViewImpl(); }
std::vector<Entity> getScriptView() { return Manager::getInstance().getScriptViewImpl(); }

Entity getSelectedEntity() {
    if (Entity(_selectedEntity).exists())
        return _selectedEntity;
    else
        return -1;
}
void setSelectedEntity(Entity eid) { _selectedEntity = eid; }

// Memory management
void createDefault() { Manager::getInstance().createDefaultImpl(); }

void clear() { Manager::getInstance().clearImpl(); }

} // namespace atta::component
