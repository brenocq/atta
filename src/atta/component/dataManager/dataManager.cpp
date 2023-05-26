//--------------------------------------------------
// Atta Component Module
// dataManager.cpp
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/dataManager/dataManager.h>

namespace atta::component {

ATTA_CPU_GPU uint8_t* DataManager::addComponent(Entity entity, ComponentId cid) {}

ATTA_CPU_GPU void DataManager::removeComponent(Entity entity, ComponentId cid) {}

ATTA_CPU_GPU uint8_t* DataManager::getComponent(Entity entity, ComponentId cid) {}

ATTA_CPU_GPU Entity DataManager::createEntity(EntityId entity) {}

ATTA_CPU_GPU void DataManager::destroyEntity(Entity entity) {}

} // namespace atta::component
