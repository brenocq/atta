//--------------------------------------------------
// Atta Component Module
// dataManager.cpp
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/dataManager/dataManager.h>

namespace atta::component {

// clang-format off
#define CHECK_ENTITY(eid, x) \
    if (!validEntity(eid)) { \
        LOG_WARN("cmp::DataManager", "Trying to call [w]$0[] with entity [w]$1[] that is not valid", __func__, eid); \
        return x; \
    }
#define CHECK_COMPONENT(cid, x) \
    if (!validComponent(cid)) { \
        LOG_WARN("cmp::DataManager", "Trying to call [w]$0[] with component [w]$1[] that is not valid", __func__, cid); \
        if constexpr (std::is_void_v<decltype(__func__)>) \
        return x; \
    }
// clang-format on

ATTA_CPU void DataManager::initEntityPool() {
    _currentEntity = 0;
    for (EntityBlock& eb : _entityPool) {
        eb.exist = false;
        for (size_t i = 0; i < eb.components.size(); i++)
            eb.components[i] = nullptr;
    }
}

//-----------------------------------//
//--------- Entity Interface --------//
//-----------------------------------//
ATTA_CPU_GPU EntityId DataManager::createEntity(EntityId eid) {
    EntityId createdId = -1;
    if (validEntity(eid) && !entityExists(eid)) {
        // Create entity with specific id
        createdId = eid;
    } else {
        // Create entity with unspecified id
        EntityId start = _currentEntity;

        // Find first free entity
        do {
            if (!_entityPool[_currentEntity].exist) {
                createdId = _currentEntity;
                break;
            }
            _currentEntity = (_currentEntity + 1) % _entityPool.size();
        } while (_currentEntity != start);
    }

    // Create entity
    if (createdId >= 0) {
        _entityPool[createdId].exist = true;
        _currentEntity = (createdId + 1) % _entityPool.size();
    }

    return createdId;
}

ATTA_CPU_GPU void DataManager::destroyEntity(EntityId eid) {
    CHECK_ENTITY(eid, );

    if (entityExists(eid)) {
        // Destroy entity
        _entityPool[eid].exist = false;

        // Remove components
        for (ComponentId cid = 0; cid < maxComponents; cid++)
            if (_entityPool[eid][cid] != nullptr)
                removeComponent(eid, cid);
    }
}

ATTA_CPU_GPU bool DataManager::entityExists(EntityId eid) {
    CHECK_ENTITY(eid, false);

    return _entityPool[eid].exist;
}

ATTA_CPU_GPU bool DataManager::validEntity(EntityId eid) { return eid >= 0 && eid < _entityPool.size(); }

ATTA_CPU std::vector<EntityId> DataManager::getEntities() const {
    std::vector<EntityId> entities;
    for (size_t i = 0; i < maxEntities; i++)
        if (_entityPool[i].exist)
            entities.push_back(EntityId{i});
    return entities;
}

//-----------------------------------//
//------- Component Interface -------//
//-----------------------------------//
ATTA_CPU_GPU Component* DataManager::addComponent(EntityId eid, ComponentId cid) {
    CHECK_ENTITY(eid, nullptr);
    CHECK_COMPONENT(cid, nullptr);

    if (entityExists(eid) && _entityPool[eid][cid] == nullptr)
        _entityPool[eid][cid] = _componentPools[cid].alloc();
    return _entityPool[eid][cid];
}

ATTA_CPU_GPU void DataManager::removeComponent(EntityId eid, ComponentId cid) {
    CHECK_ENTITY(eid, );
    CHECK_COMPONENT(cid, );

    if (entityExists(eid) && _entityPool[eid][cid] != nullptr) {
        _componentPools[cid].free(_entityPool[eid][cid]);
        _entityPool[eid][cid] = nullptr;
    }
}

ATTA_CPU_GPU Component* DataManager::getComponent(EntityId eid, ComponentId cid) {
    CHECK_ENTITY(eid, nullptr);
    CHECK_COMPONENT(cid, nullptr);

    if (entityExists(eid))
        return _entityPool[eid][cid];
}

ATTA_CPU std::vector<ComponentId> DataManager::getComponents(EntityId eid) {
    std::vector<ComponentId> components;

    for (ComponentId cid = 0; cid < maxComponents; cid++)
        if (_entityPool[eid][cid] != nullptr)
            components.push_back(cid);

    return components;
}

ATTA_CPU_GPU bool DataManager::validComponent(ComponentId component) {
    return component >= 0 && component < _componentPools.size() && _componentPools[component].isAllocated();
}

//-----------------------------------//
//----------- EntityBlock -----------//
//-----------------------------------//
Component*& DataManager::EntityBlock::operator[](size_t index) { return components[index]; }

} // namespace atta::component
