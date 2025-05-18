//--------------------------------------------------
// Atta Component Module
// manager.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/components.h>
#include <atta/component/factory.h>
#include <atta/component/interface.h>
#include <atta/component/manager.h>
#include <atta/event/events/createComponent.h>
#include <atta/event/events/createEntity.h>
#include <atta/event/events/deleteComponent.h>
#include <atta/event/events/deleteEntity.h>
#include <atta/event/events/imageLoad.h>
#include <atta/event/events/meshLoad.h>
#include <atta/event/events/scriptTarget.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/material.h>
#include <cstring>

namespace atta::component {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    _maxEntities = maxEntities;

    //----- Module Memory -----//
    // Get main memory
    memory::Allocator* mainAllocator = memory::getAllocator(SSID("MainAllocator"));
    size_t size = 128 * 1024 * 1024; // 128MB
    // Alloc memory inside main memory
    uint8_t* componentMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
    ASSERT(componentMemory != nullptr, "Could not allocate component system memory");

    // Create new allocator from component memory
    _allocator = new memory::StackAllocator(componentMemory, size);
    memory::registerAllocator(SSID("ComponentAllocator"), static_cast<memory::Allocator*>(_allocator));

    createEntityPool();
    createComponentPoolsFromRegistered();
    // Can be used to free all custom component allocators (useful when reloading a project)
    // Because the Manager::startUp method is called before any project is loaded, only atta components were created at this point
    _customComponentsMarker = _allocator->getMarker();
    _numAttaComponents = _componentRegistries.size();

    event::subscribe<event::SimulationStart>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::SimulationStop>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::MeshLoad>(BIND_EVENT_FUNC(Manager::onMeshEvent));
    event::subscribe<event::ImageLoad>(BIND_EVENT_FUNC(Manager::onImageEvent));
    event::subscribe<event::ScriptTarget>(BIND_EVENT_FUNC(Manager::onScriptEvent));

    // Default entity
    createDefaultImpl();
}

void Manager::shutDownImpl() {}

void Manager::createDefaultImpl() {
    // Cube entity
    Entity cube = createEntity();
    cube.add<Transform>();
    cube.add<Name>()->set("Cube");
    cube.add<Mesh>()->set("meshes/cube.obj");

    resource::Material::CreateInfo mInfo{};
    mInfo.color = {0.5, 0.5, 0.5};
    resource::Material* rmt = resource::create<resource::Material>("Material", mInfo);
    cube.add<Material>()->set(rmt);

    // Light entity
    Entity light = createEntity();
    Transform* t = light.add<Transform>();
    t->position = {0.6, 0.6, 0.6};
    t->scale = {0.05, 0.05, 0.05};
    light.add<Name>()->set("Point light");
    light.add<Mesh>()->set("meshes/sphere.obj");
    light.add<PointLight>();
}

void Manager::createEntityPool() {
    const size_t entityMemorySize = ceil(_maxEntities / 8.0f) + sizeof(EntityBlock) * _maxEntities; // Memory for bitmap + pool blocks

    // Allocate from component system memory
    uint8_t* entityMemory =
        reinterpret_cast<uint8_t*>(_allocator->allocBytes(entityMemorySize, sizeof(EntityBlock))); // TODO Probably not aligned because of the bitmap
    ASSERT(entityMemory != nullptr, "Could not allocate component system entity memory");

    // Create entity bitmap pool allocator
    memory::registerAllocator(SSID("Component_EntityAllocator"),
                              static_cast<memory::Allocator*>(new memory::BitmapAllocator(entityMemory, entityMemorySize, sizeof(EntityBlock))));
}

Entity Manager::createEntityImpl(EntityId entity, size_t quantity) {
    memory::BitmapAllocator* pool = memory::getAllocator<memory::BitmapAllocator>(SID("Component_EntityAllocator"));

    if (entity != -1 && pool->getBlockBit(entity)) {
        LOG_WARN("component::Manager", "Trying to create entity [w]$0[] that already exists", entity);
        return Entity(-1);
    }

    // Alloc entity
    EntityBlock* e = nullptr;
    if (entity == -1)
        e = pool->alloc<EntityBlock>(quantity);
    else
        e = pool->allocAtIndex<EntityBlock>(entity, quantity);
    ASSERT(e != nullptr, "Could not create entity, probably atta memory is full");

    // Initialize entity component pointers
    for (EntityBlock* it = e; it < e + quantity; it++)
        for (size_t i = 0; i < sizeof(EntityBlock) / sizeof(void*); i++)
            it->components[i] = nullptr;

    // Calculate entityId (index inside pool memory)
    EntityId eid = static_cast<EntityId>(pool->getIndex(e));

    for (EntityId i = eid; i < EntityId(eid + quantity); i++) {
        _noPrototypeView.insert(i);
        _entities.insert(i);
    }

    // Publish create entity event
    event::CreateEntity event;
    event.entityId = eid;
    event::publish(event);

    return Entity(eid);
}

EntityId Manager::createClonesImpl(size_t quantity) {
    EntityId eid = createEntityImpl(-1, quantity);
    for (EntityId i = eid; i < eid + EntityId(quantity); i++)
        _cloneView.insert(i);
    return eid;
}

void Manager::deleteEntityImpl(Entity entity) {
    EntityId eid = entity.getId();

    // Get entity
    memory::BitmapAllocator* epool = memory::getAllocator<memory::BitmapAllocator>(SID("Component_EntityAllocator"));
    EntityBlock* e = epool->getBlock<EntityBlock>(eid);
    ASSERT(e != nullptr, "Trying to delete entity [w]$0[] that never was created", eid);

    // Delete children and remove parent relationship
    Relationship* r = getComponent<Relationship>(eid);
    if (r) {
        if (r->getParent() != -1)
            r->removeParent(r->getParent(), eid);

        auto children = r->getChildren();
        for (auto child : children)
            deleteEntity(child);
    }

    // Delete allocated components
    for (unsigned i = 0; i < _componentRegistries.size(); i++)
        if (e->components[i] != nullptr)
            removeComponentByIdImpl(_componentRegistries[i]->getId(), entity);

    // Unselect
    if (_selectedEntity == eid)
        _selectedEntity = -1;

    // Free entity
    epool->free<EntityBlock>(e);
    _entities.erase(eid);
    _noPrototypeView.erase(eid);
    _cloneView.erase(eid);
    _scriptView.erase(eid);

    // Publish delete entity event
    event::DeleteEntity event;
    event.entityId = eid;
    event::publish(event);
}

void Manager::deleteEntityOnlyImpl(Entity entity) {
    EntityId eid = entity.getId();

    // Get entity
    memory::BitmapAllocator* epool = memory::getAllocator<memory::BitmapAllocator>(SID("Component_EntityAllocator"));
    EntityBlock* e = epool->getBlock<EntityBlock>(eid);
    ASSERT(e != nullptr, "Trying to delete entity [w]$0[] that never was created", eid);

    // Unselect
    if (_selectedEntity == eid)
        _selectedEntity = -1;

    // Free entity
    epool->free<EntityBlock>(e);
    _entities.erase(eid);
    _noPrototypeView.erase(eid);
    _cloneView.erase(eid);
    _scriptView.erase(eid);

    // Publish delete entity event
    event::DeleteEntity event;
    event.entityId = eid;
    event::publish(event);
}

Entity Manager::copyEntityImpl(Entity entity) {
    EntityId eid = entity.getId();

    // Get entity
    EntityBlock* e = getEntityBlock(eid);
    ASSERT(e != nullptr, "Trying to copy entity [w]$0[] that does not exists", eid);

    Entity newEntity = createEntity();

    // Delete allocated components
    for (auto compReg : _componentRegistries) {
        // If base entity has this component
        Component* baseComponent = getComponentById(compReg->getId(), entity);
        if (baseComponent) {
            Component* component = addComponentById(compReg->getId(), newEntity);

            // TODO implement component copy operator
            if (compReg->getId() != TypedComponentRegistry<Relationship>::getInstance().getId())
                memcpy(component, baseComponent, compReg->getSizeof());
            else {
                Relationship* baseR = static_cast<Relationship*>(baseComponent);
                Relationship* copyR = static_cast<Relationship*>(component);
                if (baseR->getParent() != -1)
                    copyR->setParent(baseR->getParent(), newEntity);
            }
        }
    }

    return newEntity;
}

void Manager::clearImpl() {
    // Clear entities
    memory::BitmapAllocator* epool = memory::getAllocator<memory::BitmapAllocator>(SID("Component_EntityAllocator"));
    epool->clear();

    // Clear entity view
    _entities.clear();
    _noPrototypeView.clear();
    _cloneView.clear();
    _scriptView.clear();

    // Clear components
    for (auto reg : _componentRegistries)
        getComponentAllocator(reg)->clear();
}

memory::BitmapAllocator* Manager::getComponentAllocator(ComponentRegistry* compReg) {
    return memory::getAllocator<memory::BitmapAllocator>(COMPONENT_POOL_SID_BY_NAME(compReg->getTypeidName()));
}

void Manager::registerComponentImpl(ComponentRegistry* componentRegistry) {
    ASSERT(_componentRegistries.size() < maxRegisteredComponents,
           "More components than it is possible to store inside the entityBlock, maximum is $0", maxRegisteredComponents);

    // Check if not already registered
    int oldIndex = -1;
    for (unsigned i = 0; i < _componentRegistries.size() && i < _componentRegistriesBackupInfo.size(); i++)
        if (componentRegistry->getTypeidHash() == _componentRegistriesBackupInfo[i].typeidHash) {
            oldIndex = i;
            break;
        }

    if (oldIndex == -1) {
        componentRegistry->setIndex(_componentRegistries.size());
        _componentRegistries.push_back(componentRegistry);

        // Push new to registered backup (will be updated later because the Description data may not be available while the components are being
        // registered) Need to keep track of this because registerComponentImpl can be called multiple times for the same component (one time for each
        // translation unit). We need to be sure that will not push the same componentRegistry twice
        _componentRegistriesBackupInfo.push_back(
            {componentRegistry->getTypeidHash(), ComponentDescription{componentRegistry->getTypeidName()}, false});
    } else {
        // XXX Not removing old components
        // LOG_DEBUG("component::Manager", "Reloading component [w]$0[]", componentRegistry->getTypeidName());
        // If the layout changed: (TODO)
        //   1. Save entities with old data
        //   2. Deallocate old memory
        //   3. Allocate new memory
        //   4. Change entity component pointer and copy+format component data
        // If the layout was not changed:
        //   No nothing
        if (_componentRegistries[oldIndex] != componentRegistry) {
            // LOG_WARN("component::Manager", "Component registry pointer changed from $0 to $1\n new: $0", _componentRegistries[oldIndex],
            // componentRegistry);
            _componentRegistries[oldIndex] = componentRegistry;
        }

        componentRegistry->setPoolCreated(_componentRegistriesBackupInfo[oldIndex].poolCreated);
        componentRegistry->setIndex(oldIndex);
    }
}

void Manager::createComponentPoolsFromRegistered() {
    for (auto reg : _componentRegistries) {
        // TODO Remove custom component registry when it is not loaded (pointer to random data)
        if (!reg->getPoolCreated()) {
            createComponentPool(reg);
            reg->setPoolCreated(true);
        }
    }
}

void Manager::createComponentPool(ComponentRegistry* componentRegistry) {
    ComponentDescription& desc = componentRegistry->getDescription();
    std::string name = desc.name;
    unsigned sizeofT = componentRegistry->getSizeof();
    std::string typeidTName = componentRegistry->getTypeidName();
    unsigned maxCount = desc.maxInstances;

    // TODO better bitmap allocator allocation from another allocator (now need to know that implementation to implement it correctly)
    // Should not need to calculate this size
    size_t size = ceil(maxCount / 8.0f) + sizeofT * maxCount;

    uint8_t* componentMemory = reinterpret_cast<uint8_t*>(_allocator->allocBytes(size, sizeofT));
    DASSERT(componentMemory != nullptr, "Could not allocate component system memory for " + name);
    // LOG_INFO("component::Manager", "Allocated memory for component $0 ($1). $2MB ($5 instances) -> memory space:($3 $4)", name, typeidTName,
    //          maxCount * sizeofT / (1024 * 1024.0f), (void*)(componentMemory), (void*)(componentMemory + maxCount * sizeofT), maxCount);

    // Create pool allocator
    memory::registerAllocator(COMPONENT_POOL_SSID_BY_NAME(typeidTName),
                              static_cast<memory::Allocator*>(new memory::BitmapAllocator(componentMemory, size, sizeofT)));
}

void Manager::unregisterCustomComponentsImpl() {
    // Return stack pointer to the point before custom components (free custom component allocators)
    _componentRegistries.resize(_numAttaComponents);
    _allocator->rollback(_customComponentsMarker);
}

//----------------------------------------//
//--------- Remove/Add component ---------//
//----------------------------------------//
Component* Manager::addComponentByIdImpl(ComponentId id, Entity entity) {
    EntityId eid = entity.getId();
    ASSERT(eid < (int)_maxEntities, "Trying to access entity outside of range");

    // Get entity
    EntityBlock* e = getEntityBlock(eid);

    // Get component registry
    ComponentRegistry* compReg = nullptr;
    for (auto cg : _componentRegistries)
        if (cg->getId() == id) {
            compReg = cg;
            break;
        }
    ASSERT(compReg != nullptr, "Trying to add unknown component with id $0", id);
    ASSERT(e != nullptr, "Trying to add component [w]$0[] to entity [w]$1[] that was not created", compReg->getDescription().name, eid);

    if (e->components[compReg->getIndex()] != nullptr) {
        LOG_WARN("component::Manager", "Could not add component [w]$1[] to entity [w]$0[]. The entity [w]$0[] already has the component [w]$1[]", eid,
                 compReg->getDescription().name);
        return nullptr;
    }

    // Alloc component
    memory::BitmapAllocator* cpool = memory::getAllocator<memory::BitmapAllocator>(id);
    Component* component = cpool->alloc<Component>();

    if (component) {
        // Initialization
        std::vector<uint8_t> defaultInit = compReg->getDefault();
        memcpy(component, defaultInit.data(), compReg->getSizeof());

        // Remove entity from some views if it is a prototype
        if (id == COMPONENT_POOL_SID_BY_NAME(typeid(Prototype).name())) {
            _noPrototypeView.erase(eid);
            _scriptView.erase(eid);
        }

        // Add entity to script view if it is not prototype and has script component
        if (id == COMPONENT_POOL_SID_BY_NAME(typeid(Script).name())) {
            Prototype* pc = getComponent<Prototype>(eid);
            if (pc == nullptr)
                _scriptView.insert(eid);
        }

        // Add component to entity
        e->components[compReg->getIndex()] = reinterpret_cast<void*>(component);

        // Publish create component event
        event::CreateComponent event;
        event.componentId = id;
        event.entityId = eid;
        event.component = component;
        event::publish(event);

        return component;
    } else {
        LOG_WARN("component::Manager", "Could not allocate component $0 for entity $1", compReg->getDescription().name, entity);
        return nullptr;
    }
}

Component* Manager::addComponentPtrImpl(Entity entity, unsigned index, uint8_t* component) {
    EntityId eid = entity.getId();
    DASSERT(eid < (int)_maxEntities, "Trying to access entity outside of range");

    // Get entity
    EntityBlock* e = getEntityBlock(eid);
    ASSERT(e != nullptr, "Trying to add component pointer to entity [w]$0[] that was not created", eid);

    // Add pointer to entity
    if (e->components[index] == nullptr)
        e->components[index] = reinterpret_cast<void*>(component);
    else
        LOG_WARN("component::Manager", "Trying to override entity [w]$0[] component pointer. Returning already allocated one", eid);

    ComponentId id = _componentRegistries[index]->getId();

    // Remove entity from some views if it is a prototype
    if (id == COMPONENT_POOL_SID_BY_NAME(typeid(Prototype).name())) {
        _noPrototypeView.erase(eid);
        _scriptView.erase(eid);
    }

    // Add entity to script view if it is not prototype and has script component
    if (id == COMPONENT_POOL_SID_BY_NAME(typeid(Script).name())) {
        Prototype* pc = getComponent<Prototype>(eid);
        if (pc == nullptr)
            _scriptView.insert(eid);
    }

    Component* componentPtr = reinterpret_cast<Component*>(e->components[index]);

    // Publish create component event
    event::CreateComponent event;
    event.componentId = id;
    event.entityId = eid;
    event.component = componentPtr;
    event::publish(event);

    return componentPtr;
}

void Manager::removeComponentByIdImpl(ComponentId id, Entity entity) {
    EntityId eid = entity.getId();
    DASSERT(eid < (int)_maxEntities, "Trying to access entity outside of range");
    // TODO View inconsistencity if more than one script/prototype component was added

    // Get entity
    EntityBlock* e = getEntityBlock(eid);
    ASSERT(e != nullptr, "Trying to remove component from entity [w]$0[] that was not created", eid);

    // Get component registry
    ComponentRegistry* compReg = nullptr;
    for (auto cg : _componentRegistries)
        if (cg->getId() == id) {
            compReg = cg;
            break;
        }
    ASSERT(compReg != nullptr, "Trying to add unknown component with id $0", id);

    // Get component pool
    memory::BitmapAllocator* cpool = memory::getAllocator<memory::BitmapAllocator>(id);

    // Free component
    cpool->free(e->components[compReg->getIndex()]);

    // Clear entity block
    e->components[compReg->getIndex()] = nullptr;

    if (id == COMPONENT_POOL_SID_BY_NAME(typeid(Script).name()))
        _scriptView.erase(eid);
    if (id == COMPONENT_POOL_SID_BY_NAME(typeid(Prototype).name()))
        _noPrototypeView.insert(eid);

    // Publish delete component event
    event::DeleteComponent event;
    event.componentId = id;
    event.entityId = eid;
    event::publish(event);
}

//----------------------------------------//
//--------- Get entity component ---------//
//----------------------------------------//
Component* Manager::getComponentByIdImpl(ComponentId id, Entity entity) {
    EntityId eid = entity.getId();
    // Get component registry
    ComponentRegistry* compReg = nullptr;
    for (auto cg : _componentRegistries)
        if (cg->getId() == id) {
            compReg = cg;
            break;
        }
    ASSERT(compReg != nullptr, "Trying to add unknown component with id $0", id);

    return getComponentByIndex(compReg->getIndex(), eid);
}

Component* Manager::getComponentByIndex(unsigned index, Entity entity) {
    EntityId eid = entity.getId();
    DASSERT(eid < (int)_maxEntities, "Trying to access entity outside of range");
    DASSERT(index < maxRegisteredComponents, "Trying to access component by index outside of range");

    // Get entity
    EntityBlock* e = getEntityBlock(eid);
    ASSERT(e != nullptr, "Trying to get component [w]$1[] from entity [w]$0[], but this entity was not created", entity,
           _componentRegistries[index]->getDescription().name);

    // Return component
    return reinterpret_cast<Component*>(e->components[index]);
}

std::vector<Component*> Manager::getComponentsImpl(Entity entity) {
    EntityId eid = entity.getId();
    DASSERT(eid < (int)_maxEntities, "Trying to access entity outside of range");

    EntityBlock* e = getEntityBlock(eid);
    ASSERT(e != nullptr, "Trying to remove component from entity [w]$0[] that was not created", eid);

    std::vector<Component*> components;
    for (size_t i = 0; i < sizeof(EntityBlock) / sizeof(void*); i++)
        components.push_back(reinterpret_cast<Component*>(e->components[i]));

    return components;
}

//----------------------------------------//
//----------------- Views ----------------//
//----------------------------------------//
std::vector<EntityId> Manager::getEntitiesViewImpl() { return std::vector<EntityId>(_entities.begin(), _entities.end()); }

std::vector<EntityId> Manager::getNoPrototypeViewImpl() { return std::vector<EntityId>(_noPrototypeView.begin(), _noPrototypeView.end()); }

std::vector<EntityId> Manager::getCloneViewImpl() { return std::vector<EntityId>(_cloneView.begin(), _cloneView.end()); }

std::vector<EntityId> Manager::getNoCloneViewImpl() {
    std::vector<EntityId> noClones;
    for (auto entity : getEntitiesView())
        if (_cloneView.find(entity) == _cloneView.end())
            noClones.push_back(entity);
    return noClones;
}

std::vector<EntityId> Manager::getScriptViewImpl() { return std::vector<EntityId>(_scriptView.begin(), _scriptView.end()); }

//----------------------------------------//
//----------- Memory Management ----------//
//----------------------------------------//
Manager::EntityBlock* Manager::getEntityBlock(EntityId eid) {
    memory::BitmapAllocator* epool = memory::getAllocator<memory::BitmapAllocator>(SID("Component_EntityAllocator"));
    return epool->getBlock<EntityBlock>(eid);
}

//----------------------------------------//
//---------------- Factory ---------------//
//----------------------------------------//
void Manager::createFactories() {
    // Create factory for each entity that has prototype
    for (EntityId entity : getEntitiesView()) {
        Prototype* prototype = getComponentImpl<Prototype>(entity);
        if (prototype && prototype->maxClones > 0) {
            _factories.emplace_back(entity);
            _factories.back().createClones();
        }
    }
}

void Manager::destroyFactories() {
    for (int i = _factories.size() - 1; i >= 0; i--)
        _factories[i].destroyClones();
    _factories.clear();
}

Factory* Manager::getFactoryImpl(Entity prototype) {
    for (Factory& factory : _factories)
        if (factory.getPrototype() == prototype)
            return &factory;
    LOG_WARN("component::Manager", "Trying to get factory from entity [w]$0[] that is not a prototype", prototype);
    return nullptr;
}

//----------------------------------------//
//--------------- Events -----------------//
//----------------------------------------//
void Manager::onSimulationStateChange(event::Event& event) {
    if (event.getType() == event::SimulationStart::type) {
        createFactories();
    } else if (event.getType() == event::SimulationStop::type) {
        destroyFactories();
    } else {
        LOG_WARN("component::Manager", "Received simulation event that was not be handled (type=[w]$0[])", event.getType());
    }
}

void Manager::onMeshEvent(event::Event& event) {
    switch (event.getType()) {
        case event::MeshLoad::type: {
            event::MeshLoad& e = reinterpret_cast<event::MeshLoad&>(event);
            bool found = false;
            for (std::string op : TypedComponentRegistry<Mesh>::description->attributeDescriptions[0].options)
                if (e.sid == op) {
                    found = true;
                    break;
                }

            if (!found)
                TypedComponentRegistry<Mesh>::description->attributeDescriptions[0].options.push_back(e.sid.getString());

            break;
        }
        default:
            break;
    }
}

void Manager::onImageEvent(event::Event& event) {
    switch (event.getType()) {
        case event::ImageLoad::type: {
            event::ImageLoad& e = reinterpret_cast<event::ImageLoad&>(event);

            // Update environment light options
            {
                bool found = false;
                for (std::string op : TypedComponentRegistry<EnvironmentLight>::description->attributeDescriptions[0].options)
                    if (e.sid == op) {
                        found = true;
                        break;
                    }

                if (!found) {
                    TypedComponentRegistry<EnvironmentLight>::description->attributeDescriptions[0].options.push_back(e.sid.getString());
                }
            }

            break;
        }
        default:
            break;
    }
}

void Manager::onScriptEvent(event::Event& event) {
    event::ScriptTarget& e = reinterpret_cast<event::ScriptTarget&>(event);

    TypedComponentRegistry<Script>::description->attributeDescriptions[0].options.clear();
    for (StringId script : e.scriptSids)
        TypedComponentRegistry<Script>::description->attributeDescriptions[0].options.push_back(script.getString());

    // Created pool to new components if necessary
    createComponentPoolsFromRegistered();

    // Update backup info (only now we can guarantee that the description data is available)
    _componentRegistriesBackupInfo.clear();
    for (auto reg : _componentRegistries) {
        ComponentRegistryBackupInfo crbi;
        crbi.typeidHash = reg->getTypeidHash();
        crbi.description.name = reg->getDescription().name;
        crbi.description.attributeDescriptions = reg->getDescription().attributeDescriptions;
        crbi.poolCreated = true;
        _componentRegistriesBackupInfo.push_back(crbi);
    }
}

} // namespace atta::component
