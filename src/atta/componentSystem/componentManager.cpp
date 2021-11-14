//--------------------------------------------------
// Atta Component System
// componentManager.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/components.h>
#include <atta/componentSystem/factory.h>
#include <atta/eventSystem/events/simulationStartEvent.h>
#include <atta/eventSystem/events/simulationStopEvent.h>
#include <atta/eventSystem/events/meshLoadEvent.h>
#include <atta/eventSystem/events/textureLoadEvent.h>
#include <atta/eventSystem/events/scriptTargetEvent.h>
#include <cstring>

namespace atta
{
    void ComponentManager::startUpImpl()
    {
        _maxEntities = 1024;

        //----- System Memory -----//
        // Get main memory
        Allocator* mainAllocator = MemoryManager::getAllocator(SSID("MainAllocator"));
        size_t size = 512*1024*1024;// 512MB
        // Alloc memory inside main memory
        uint8_t* componentMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
        ASSERT(componentMemory != nullptr, "Could not allocate component system memory");

        // Create new allocator from component memory
        _allocator = new StackAllocator(componentMemory, size);
        MemoryManager::registerAllocator(SSID("ComponentAllocator"), static_cast<Allocator*>(_allocator));

        createEntityPool();
        createComponentPoolsFromRegistered();
        // Can be used to free all custom component allocators (useful when reloading a project)
        // Because the ComponentManager::startUp method is called before any project is loaded, only atta components were created at this point
        _customComponentsMarker = _allocator->getMarker();
        _numAttaComponents = _componentRegistries.size();

        EventManager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(ComponentManager::onSimulationStateChange));
        EventManager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(ComponentManager::onSimulationStateChange));
        EventManager::subscribe<MeshLoadEvent>(BIND_EVENT_FUNC(ComponentManager::onMeshEvent));
        EventManager::subscribe<TextureLoadEvent>(BIND_EVENT_FUNC(ComponentManager::onTextureEvent));
        EventManager::subscribe<ScriptTargetEvent>(BIND_EVENT_FUNC(ComponentManager::onScriptEvent));

        // Default entity
        createDefaultImpl();
    }

    void ComponentManager::shutDownImpl()
    {

    }

    void ComponentManager::createDefaultImpl()
    {
        // Cube entity
        EntityId cube = createEntity();
        TransformComponent* t = addEntityComponent<TransformComponent>(cube);

        NameComponent* n = addEntityComponent<NameComponent>(cube);
        strcpy(n->name, "Cube");

        MeshComponent* m = addEntityComponent<MeshComponent>(cube);
        m->sid = StringId("meshes/cube.obj");

        MaterialComponent* mt = addEntityComponent<MaterialComponent>(cube);
        mt->albedo = { 0.5, 0.5, 0.5 };

        // Light entity
        EntityId light = createEntity();
        t = addEntityComponent<TransformComponent>(light);
        t->position = { -0.6, -0.6, 0.6 };
        t->scale = { 0.05, 0.05, 0.05 };

        n = addEntityComponent<NameComponent>(light);
        strcpy(n->name, "Point light");

        m = addEntityComponent<MeshComponent>(light);
        m->sid = StringId("meshes/sphere.obj");

        addEntityComponent<PointLightComponent>(light);
    }

    void ComponentManager::createEntityPool()
    {
        const size_t entityMemorySize = (sizeof(EntityId)+sizeof(Entity))*_maxEntities;// TODO Entity objects may need to me aligned

        //LOG_VERBOSE("ComponentManager", "Allocating memory for entities. $0MB\n - Entity block size: $1.\n - Limits: \n    - $2 entities\n    - $3 components per entity", entityMemorySize/(1024*1024.0f), sizeof(EntityId), _maxEntities, sizeof(Entity)/sizeof(void*));

        // Allocate from component system memory
        uint8_t* entityMemory = reinterpret_cast<uint8_t*>(_allocator->allocBytes(entityMemorySize, sizeof(Entity)));
        ASSERT(entityMemory != nullptr, "Could not allocate component system entity memory");

        // Use start of the allocate memory for the dense list
        _denseList = reinterpret_cast<EntityId*>(entityMemory);
        _denseListSize = 0;

        // Create entity pool allocator
        uint8_t* startEntityPool = entityMemory+sizeof(EntityId)*_maxEntities;
        MemoryManager::registerAllocator(SSID("Component_EntityAllocator"), 
                static_cast<Allocator*>(new PoolAllocatorT<Entity>(startEntityPool, _maxEntities)));
    }

    EntityId ComponentManager::createEntity() { return getInstance().createEntityImpl(); }
    EntityId ComponentManager::createEntityImpl()
    {
        PoolAllocatorT<Entity>* pool = MemoryManager::getAllocator<PoolAllocatorT<Entity>>(SID("Component_EntityAllocator"));

        // Alloc entity
        Entity* e = pool->alloc();

        // Initialize entity component pointers
        for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
            e->components[i] = nullptr;

        // Calculate entityId (index inside pool memory)
        EntityId eid = static_cast<EntityId>(pool->getIndex(e));

        // Add entity to dense list
        _denseList[_denseListSize++] = eid;

        _noPrototypeView.insert(eid);

        return eid;
    }

    EntityId ComponentManager::createClone() { return getInstance().createCloneImpl(); }
    EntityId ComponentManager::createCloneImpl()
    {
        EntityId eid = createEntityImpl();
        _cloneView.insert(eid);
        return eid;
    }

    void ComponentManager::destroyEntity(EntityId entity) { return getInstance().destroyEntityImpl(entity); }
    void ComponentManager::destroyEntityImpl(EntityId entity)
    {
        LOG_WARN("ComponentManager", "destroyEntity(EntityId) not implemented yet!");
    }
    
    void ComponentManager::destroyEntityOnly(EntityId entity) { return getInstance().destroyEntityOnlyImpl(entity); }
    void ComponentManager::destroyEntityOnlyImpl(EntityId entity)
    {
        PoolAllocatorT<Entity>* epool = MemoryManager::getAllocator<PoolAllocatorT<Entity>>(SID("Component_EntityAllocator"));
    
        // Get entity
        Entity* e = epool->getBlock(entity);

        // Remove entity component pointers
        for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
            e->components[i] = nullptr;

        // Remove entity from dense list
        for(unsigned i = 0; i < _denseListSize; i++)
            if(_denseList[i] == entity)
            {
                // Swap this entity with last entity and reduce list size
                _denseList[i] = _denseList[_denseListSize-1];
                _denseListSize--;
                break;
            }
    }

    void ComponentManager::clearImpl()
    {
        // Clear entities
        PoolAllocatorT<Entity>* epool = MemoryManager::getAllocator<PoolAllocatorT<Entity>>(SID("Component_EntityAllocator"));
        epool->clear();

        // Clear entity view
        _noPrototypeView.clear();
        _cloneView.clear();
        _scriptView.clear();
        _denseListSize = 0;

        // Clear components
        for(auto reg : _componentRegistries)
            getComponentAllocator(reg)->clear();
    }

    PoolAllocator* ComponentManager::getComponentAllocator(ComponentRegistry* compReg)
    {
        return MemoryManager::getAllocator<PoolAllocator>(COMPONENT_POOL_SID_BY_NAME(compReg->getTypeidName()));
    }

    void ComponentManager::registerComponentImpl(ComponentRegistry* componentRegistry)
    {
        _componentRegistries.push_back(componentRegistry);
    }

    void ComponentManager::createComponentPoolsFromRegistered()
    {
        for(auto reg : _componentRegistries)
            createComponentPool(reg);
    }

    void ComponentManager::createComponentPool(ComponentRegistry* componentRegistry)
    {
        ComponentRegistry::Description desc = componentRegistry->getDescription();
        std::string name = desc.type;
        unsigned sizeofT = componentRegistry->getSizeof();
        std::string typeidTName = componentRegistry->getTypeidName();
        size_t typeidTHash = componentRegistry->getTypeidHash();
        unsigned maxCount = desc.maxInstances;

        // TODO better pool allocator allocation from another one (now need to know that implementation to implement it correctly)
        // Should not need to calculate this size
        size_t size = std::max((unsigned)sizeof(void*), sizeofT);

        uint8_t* componentMemory = reinterpret_cast<uint8_t*>(_allocator->allocBytes(maxCount*size, size));
        DASSERT(componentMemory != nullptr, "Could not allocate component system memory for " + name);
        //LOG_INFO("Component Manager", "Allocated memory for component $0 ($1). $2MB ($5 instances) -> memory space:($3 $4)", 
        //        name, typeidTName, maxCount*sizeofT/(1024*1024.0f), (void*)(componentMemory), (void*)(componentMemory+maxCount*sizeofT), maxCount);

        // Create pool allocator
        MemoryManager::registerAllocator(COMPONENT_POOL_SSID_BY_NAME(typeidTName), static_cast<Allocator*>(new PoolAllocator(componentMemory, maxCount, sizeofT)));
    }

    void ComponentManager::unregisterCustomComponentsImpl()
    {
        // Return stack pointer to the point before custom components (free custom component allocators)
        _componentRegistries.resize(_numAttaComponents);
        _allocator->rollback(_customComponentsMarker);
    }

    //----------------------------------------//
    //--------- Remove/Add component ---------//
    //----------------------------------------//
    Component* ComponentManager::addEntityComponentByIdImpl(ComponentId id, EntityId entity)
    {
        DASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");
        // TODO Check if entity was created, if this entity was not created, this will break the pool allocator

        // Get entity
        PoolAllocatorT<Entity>* epool = MemoryManager::getAllocator<PoolAllocatorT<Entity>>(SID("Component_EntityAllocator"));
        Entity* e = epool->getBlock(entity);

        int freeComponentSlot = -1;
        for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
        {
            if(e->components[i] == nullptr)
            {
                freeComponentSlot = i;
                break;
            }
        }

        if(freeComponentSlot == -1)
        {
            LOG_WARN("ComponentManager", "Could not add component $0 to entity $1", id, entity);
            return nullptr;
        }

        // Alloc component
        PoolAllocator* cpool = MemoryManager::getAllocator<PoolAllocator>(id);
        Component* component = reinterpret_cast<Component*>(cpool->alloc());

        if(component)
        {
            // Initialization
            for(auto compReg : _componentRegistries)
                if(compReg->getId() == id)
                {
                    std::vector<uint8_t> defaultInit = compReg->getDefault();
                    uint8_t* data = reinterpret_cast<uint8_t*>(component);
                    for(uint8_t val : defaultInit)
                    {
                        *data = val;
                        data++;
                    }
                    memcpy(component, defaultInit.data(), compReg->getSizeof());

                    // Remove entity from some views if it is a prototype
                    if(compReg->getId() == COMPONENT_POOL_SID_BY_NAME(typeid(PrototypeComponent).name()))
                    {
                        _noPrototypeView.erase(entity);
                        _scriptView.erase(entity);
                    }

                    // Add entity to script view if it is not prototype and has script component
                    if(compReg->getId() == COMPONENT_POOL_SID_BY_NAME(typeid(ScriptComponent).name()))
                    {
                        PrototypeComponent* pc = getEntityComponent<PrototypeComponent>(entity);
                        if(pc == nullptr)
                            _scriptView.insert(entity);
                    }

                    break;
                }

            // Add component to entity
            e->components[freeComponentSlot] = reinterpret_cast<void*>(component);

            return component;
        }
        else
        {
            LOG_WARN("ComponentManager", "Could not allocate component $0 for entity $1", id, entity);
            return nullptr;
        }
    }

    Component* ComponentManager::addEntityComponentPtrImpl(EntityId entity, uint8_t* component)
    {
        DASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");
        // TODO Check if entity was created, if this entity was not created, this will break the pool allocator

        // Get entity
        PoolAllocatorT<Entity>* epool = MemoryManager::getAllocator<PoolAllocatorT<Entity>>(SID("Component_EntityAllocator"));
        Entity* e = epool->getBlock(entity);

        // Find free component slot
        int freeComponentSlot = -1;
        for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
        {
            if(e->components[i] == nullptr)
            {
                freeComponentSlot = i;
                break;
            }
        }

        if(freeComponentSlot == -1)
        {
            LOG_WARN("ComponentManager", "Could not add component pointer $0 to entity $1", (void*)component, entity);
            return nullptr;
        }

        // Add component to entity
        e->components[freeComponentSlot] = reinterpret_cast<void*>(component);

        return reinterpret_cast<Component*>(component);
    }

    void ComponentManager::removeEntityComponentByIdImpl(ComponentId id, EntityId entity)
    {
        DASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");
        // TODO Check if entity was created, if this entity was not created, this will break the pool allocator
        // TODO View inconsistencity if more than one script/prototype component was added

        // Get entity
        PoolAllocatorT<Entity>* epool = MemoryManager::getAllocator<PoolAllocatorT<Entity>>(SID("Component_EntityAllocator"));
        Entity* e = epool->getBlock(entity);

        // Get component pool
        PoolAllocator* cpool = MemoryManager::getAllocator<PoolAllocator>(id);

        for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
            if(cpool->owns(e->components[i]))
            {
                // Found component to remove
                cpool->free(reinterpret_cast<void*>(e->components[i]));
                e->components[i] = nullptr;
                return;
            }

        if(id == COMPONENT_POOL_SID_BY_NAME(typeid(ScriptComponent).name()))
            _scriptView.erase(entity);
        if(id == COMPONENT_POOL_SID_BY_NAME(typeid(PrototypeComponent).name()))
            _noPrototypeView.insert(entity);
    }

    //----------------------------------------//
    //--------- Get entity component ---------//
    //----------------------------------------//
    Component* ComponentManager::getEntityComponentByIdImpl(ComponentId id, EntityId entity)
    {
        DASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");
        // TODO Check if entity was created, if this entity was not created, this will break the pool allocator

        // Get entity
        PoolAllocatorT<Entity>* epool = MemoryManager::getAllocator<PoolAllocatorT<Entity>>(SID("Component_EntityAllocator"));
        Entity* e = epool->getBlock(entity);

        // Get component pool
        Allocator* alloc = MemoryManager::getAllocator(id);

        for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
            if(alloc->owns(e->components[i]))
                return reinterpret_cast<Component*>(e->components[i]);
        return nullptr;
    }

    std::vector<Component*> ComponentManager::getEntityComponentsImpl(EntityId entity)
    {
        DASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");
        // TODO Check if entity was created, if this entity was not created, this will break the pool allocator

        // Get entity
        PoolAllocatorT<Entity>* epool = MemoryManager::getAllocator<PoolAllocatorT<Entity>>(SID("Component_EntityAllocator"));
        Entity* e = epool->getBlock(entity);

        std::vector<Component*> components;
        for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
            components.push_back(reinterpret_cast<Component*>(e->components[i]));

        return components;
    }

    //----------------------------------------//
    //----------------- Views ----------------//
    //----------------------------------------//
    std::vector<EntityId> ComponentManager::getEntitiesView() { return getInstance().getEntitiesViewImpl(); }
    std::vector<EntityId> ComponentManager::getEntitiesViewImpl()
    {
        return std::vector<EntityId>(_denseList, _denseList+_denseListSize);
    }

    std::vector<EntityId> ComponentManager::getNoPrototypeView() { return getInstance().getNoPrototypeViewImpl(); }
    std::vector<EntityId> ComponentManager::getNoPrototypeViewImpl()
    {
        return std::vector<EntityId>(_noPrototypeView.begin(), _noPrototypeView.end());
    }

    std::vector<EntityId> ComponentManager::getCloneView() { return getInstance().getCloneViewImpl(); }
    std::vector<EntityId> ComponentManager::getCloneViewImpl()
    {
        return std::vector<EntityId>(_cloneView.begin(), _cloneView.end());
    }

    std::vector<EntityId> ComponentManager::getScriptView() { return getInstance().getScriptViewImpl(); }
    std::vector<EntityId> ComponentManager::getScriptViewImpl()
    {
        return std::vector<EntityId>(_scriptView.begin(), _scriptView.end());
    }

    //----------------------------------------//
    //---------------- Factory ---------------//
    //----------------------------------------//
    void ComponentManager::createFactories()
    {
        // Create factory for each entity that has prototype
        for(EntityId entity : getEntitiesView())
        {
            PrototypeComponent* prototype = getEntityComponentImpl<PrototypeComponent>(entity);
            if(prototype && prototype->maxClones > 0)
            {
                _factories.emplace_back(entity);
                _factories.back().createClones();
            }
        }
    }

    void ComponentManager::destroyFactories()
    { 
        for(int i = _factories.size()-1; i>=0; i--)
            _factories[i].destroyClones();
        _factories.clear();
    }


    //----------------------------------------//
    //--------------- Events -----------------//
    //----------------------------------------//
    void ComponentManager::onSimulationStateChange(Event& event)
    {
        if(event.getType() == SimulationStartEvent::type)
        {
            createFactories();
        }
        else if(event.getType() == SimulationStopEvent::type)
        {
            destroyFactories();
        }
        else
        {
            LOG_WARN("ComponentManager", "Received simulation event that was not be handled (type=[w]$0[])", event.getType());
        }
    }

    void ComponentManager::onMeshEvent(Event& event)
    {
        switch(event.getType())
        {
            case MeshLoadEvent::type:
                {
                    MeshLoadEvent& e = reinterpret_cast<MeshLoadEvent&>(event);
                    TypedComponentRegistry<MeshComponent>::description.attributeDescriptions[0].options.insert(std::any(e.sid));
                    break;
                }
            default:
                break;
        }
    }

    void ComponentManager::onTextureEvent(Event& event)
    {
        switch(event.getType())
        {
            case TextureLoadEvent::type:
                {
                    TextureLoadEvent& e = reinterpret_cast<TextureLoadEvent&>(event);
                    TypedComponentRegistry<MaterialComponent>::description.attributeDescriptions[4].options.insert(std::any(e.sid));
                    TypedComponentRegistry<MaterialComponent>::description.attributeDescriptions[5].options.insert(std::any(e.sid));
                    TypedComponentRegistry<MaterialComponent>::description.attributeDescriptions[6].options.insert(std::any(e.sid));
                    TypedComponentRegistry<MaterialComponent>::description.attributeDescriptions[8].options.insert(std::any(e.sid));
                    TypedComponentRegistry<MaterialComponent>::description.attributeDescriptions[7].options.insert(std::any(e.sid));
                    break;
                }
            default:
                break;
        }
    }

    void ComponentManager::onScriptEvent(Event& event)
    {
        ScriptTargetEvent& e = reinterpret_cast<ScriptTargetEvent&>(event);

        TypedComponentRegistry<ScriptComponent>::description.attributeDescriptions[0].options.clear();
        for(StringId script : e.scriptSids)
            TypedComponentRegistry<ScriptComponent>::description.attributeDescriptions[0].options.insert(std::any(script));
    }
}
