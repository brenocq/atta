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
        _maxEntities = maxEntities;

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
        t->position = { 0.6, 0.6, 0.6 };
        t->scale = { 0.05, 0.05, 0.05 };

        n = addEntityComponent<NameComponent>(light);
        strcpy(n->name, "Point light");

        m = addEntityComponent<MeshComponent>(light);
        m->sid = StringId("meshes/sphere.obj");

        addEntityComponent<PointLightComponent>(light);
    }

    void ComponentManager::createEntityPool()
    {
        const size_t entityMemorySize = ceil(_maxEntities/8.0f) + sizeof(EntityBlock)*_maxEntities;// Memory for bitmap + pool blocks

        LOG_VERBOSE("ComponentManager", "Allocating memory for entities. $0MB\n - Entity block size: $1.\n - Limits: \n    - $2 entities\n    - $3 components per entity", entityMemorySize/(1024*1024.0f), sizeof(EntityId), _maxEntities, sizeof(EntityBlock)/sizeof(void*));

        // Allocate from component system memory
        uint8_t* entityMemory = reinterpret_cast<uint8_t*>(_allocator->allocBytes(entityMemorySize, sizeof(EntityBlock)));// TODO Probably not aligned because of the bitmap 
        ASSERT(entityMemory != nullptr, "Could not allocate component system entity memory");

        // Create entity bitmap pool allocator
        MemoryManager::registerAllocator(SSID("Component_EntityAllocator"), 
                static_cast<Allocator*>(new BitmapAllocator(entityMemory, entityMemorySize, sizeof(EntityBlock))));
    }

    EntityId ComponentManager::createEntity() { return getInstance().createEntityImpl(); }
    EntityId ComponentManager::createEntityImpl()
    {
        BitmapAllocator* pool = MemoryManager::getAllocator<BitmapAllocator>(SID("Component_EntityAllocator"));

        // Alloc entity
        EntityBlock* e = pool->alloc<EntityBlock>();

        // Initialize entity component pointers
        for(size_t i = 0; i < sizeof(EntityBlock)/sizeof(void*); i++)
            e->components[i] = nullptr;

        // Calculate entityId (index inside pool memory)
        EntityId eid = static_cast<EntityId>(pool->getIndex(e));

        _noPrototypeView.insert(eid);
        _entities.insert(eid);

        return eid;
    }

    EntityId ComponentManager::createClone() { return getInstance().createCloneImpl(); }
    EntityId ComponentManager::createCloneImpl()
    {
        EntityId eid = createEntityImpl();
        _cloneView.insert(eid);
        return eid;
    }

    void ComponentManager::deleteEntity(EntityId entity) { return getInstance().deleteEntityImpl(entity); }
    void ComponentManager::deleteEntityImpl(EntityId entity)
    {
        // Get entity
        BitmapAllocator* epool = MemoryManager::getAllocator<BitmapAllocator>(SID("Component_EntityAllocator"));
        EntityBlock* e = epool->getBlock<EntityBlock>(entity);
        ASSERT(e != nullptr, "Trying to delete entity [w]$0[] that never was created", entity);

        // Delete children and remove parent relationship
        RelationshipComponent* r = getEntityComponent<RelationshipComponent>(entity);
        if(r)
        {
            if(r->getParent() != -1)
                r->removeParent(r->getParent(), entity);

            auto children = r->getChildren();
            for(auto child : children)
                deleteEntity(child);
        }

        // Delete allocated components
        for(unsigned i = 0; i < _componentRegistries.size(); i++) 
            if(e->components[i] != nullptr)
            {
                // Get component pool
                BitmapAllocator* cpool = MemoryManager::getAllocator<BitmapAllocator>(_componentRegistries[i]->getId());

                // Free component
                cpool->free(e->components[i]);
            }

        // Unselect
        if(_selectedEntity == entity)
            _selectedEntity = -1;

        // Free entity
        epool->free<EntityBlock>(e);
        _entities.erase(entity);
        _noPrototypeView.erase(entity);
        _cloneView.erase(entity);
        _scriptView.erase(entity);
    }
    
    void ComponentManager::deleteEntityOnly(EntityId entity) { return getInstance().deleteEntityOnlyImpl(entity); }
    void ComponentManager::deleteEntityOnlyImpl(EntityId entity)
    {
        // Get entity
        BitmapAllocator* epool = MemoryManager::getAllocator<BitmapAllocator>(SID("Component_EntityAllocator"));
        EntityBlock* e = epool->getBlock<EntityBlock>(entity);
        ASSERT(e != nullptr, "Trying to delete entity [w]$0[] that never was created", entity);

        // Unselect
        if(_selectedEntity == entity)
            _selectedEntity = -1;

        // Free entity
        epool->free<EntityBlock>(e);
        _entities.erase(entity);
        _noPrototypeView.erase(entity);
        _cloneView.erase(entity);
        _scriptView.erase(entity);
    }

    void ComponentManager::clearImpl()
    {
        // Clear entities
        BitmapAllocator* epool = MemoryManager::getAllocator<BitmapAllocator>(SID("Component_EntityAllocator"));
        epool->clear();

        // Clear entity view
        _noPrototypeView.clear();
        _cloneView.clear();
        _scriptView.clear();

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
        ASSERT(_componentRegistries.size() < maxRegisteredComponents, "More components than it is possible to store inside the entityBlock, maximum is $0", maxRegisteredComponents);

        componentRegistry->setIndex(_componentRegistries.size());
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
        unsigned maxCount = desc.maxInstances;

        // TODO better bitmap allocator allocation from another allocator (now need to know that implementation to implement it correctly)
        // Should not need to calculate this size
        size_t size = ceil(maxCount/8.0f) + sizeofT*maxCount;

        uint8_t* componentMemory = reinterpret_cast<uint8_t*>(_allocator->allocBytes(size, sizeofT));
        DASSERT(componentMemory != nullptr, "Could not allocate component system memory for " + name);
        //LOG_INFO("Component Manager", "Allocated memory for component $0 ($1). $2MB ($5 instances) -> memory space:($3 $4)", 
        //        name, typeidTName, maxCount*sizeofT/(1024*1024.0f), (void*)(componentMemory), (void*)(componentMemory+maxCount*sizeofT), maxCount);

        // Create pool allocator
        MemoryManager::registerAllocator(COMPONENT_POOL_SSID_BY_NAME(typeidTName), 
                static_cast<Allocator*>(new BitmapAllocator(componentMemory, size, sizeofT)));
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
        ASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");

        // Get entity
        EntityBlock* e = getEntityBlock(entity);

        // Get component registry
        ComponentRegistry* compReg = nullptr;
        for(auto cg : _componentRegistries)
            if(cg->getId() == id)
            {
                compReg = cg; 
                break;
            }
        ASSERT(compReg != nullptr, "Trying to add unknown component with id $0", id);
        ASSERT(e != nullptr, "Trying to add component [w]$0[] to entity [w]$1[] that was not created", compReg->getDescription().type, entity);

        if(e->components[compReg->getIndex()] != nullptr)
        {
            LOG_WARN("ComponentManager", "Could not add component [w]$1[] to entity [w]$0[]. The entity [w]$0[] already has the component [w]$1[]", entity, compReg->getDescription().type);
            return nullptr;
        }

        // Alloc component
        BitmapAllocator* cpool = MemoryManager::getAllocator<BitmapAllocator>(id);
        Component* component = cpool->alloc<Component>();

        if(component)
        {
            // Initialization
            std::vector<uint8_t> defaultInit = compReg->getDefault();
            memcpy(component, defaultInit.data(), compReg->getSizeof());

            // Remove entity from some views if it is a prototype
            if(id == COMPONENT_POOL_SID_BY_NAME(typeid(PrototypeComponent).name()))
            {
                _noPrototypeView.erase(entity);
                _scriptView.erase(entity);
            }

            // Add entity to script view if it is not prototype and has script component
            if(id == COMPONENT_POOL_SID_BY_NAME(typeid(ScriptComponent).name()))
            {
                PrototypeComponent* pc = getEntityComponent<PrototypeComponent>(entity);
                if(pc == nullptr)
                    _scriptView.insert(entity);
            }

            // Add component to entity
            e->components[compReg->getIndex()] = reinterpret_cast<void*>(component);

            return component;
        }
        else
        {
            LOG_WARN("ComponentManager", "Could not allocate component $0 for entity $1", compReg->getDescription().type, entity);
            return nullptr;
        }
    }

    Component* ComponentManager::addEntityComponentPtrImpl(EntityId entity, unsigned index, uint8_t* component)
    {
        DASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");

        // Get entity
        EntityBlock* e = getEntityBlock(entity);
        ASSERT(e != nullptr, "Trying to add component pointer to entity [w]$0[] that was not created", entity);

        // Add pointer to entity
        if(e->components[index] == nullptr)
            e->components[index] = reinterpret_cast<void*>(component);
        else
            LOG_WARN("ComponentManager", "Trying to override entity [w]$0[] component pointer. Returning already allocated one", entity);

        return reinterpret_cast<Component*>(e->components[index]);
    }

    void ComponentManager::removeEntityComponentByIdImpl(ComponentId id, EntityId entity)
    {
        DASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");
        // TODO View inconsistencity if more than one script/prototype component was added

        // Get entity
        EntityBlock* e = getEntityBlock(entity);
        ASSERT(e != nullptr, "Trying to remove component from entity [w]$0[] that was not created", entity);

        // Get component registry
        ComponentRegistry* compReg = nullptr;
        for(auto cg : _componentRegistries)
            if(cg->getId() == id)
            {
                compReg = cg; 
                break;
            }
        ASSERT(compReg != nullptr, "Trying to add unknown component with id $0", id);

        // Get component pool
        BitmapAllocator* cpool = MemoryManager::getAllocator<BitmapAllocator>(id);

        // Free component
        cpool->free(e->components[compReg->getIndex()]);
        
        // Clear entity block
        e->components[compReg->getIndex()] = nullptr;

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

        // Get entity
        EntityBlock* e = getEntityBlock(entity);
        ASSERT(e != nullptr, "Trying to get component from entity [w]$0[], but this entity was not created", entity);

        // Get component registry
        ComponentRegistry* compReg = nullptr;
        for(auto cg : _componentRegistries)
            if(cg->getId() == id)
            {
                compReg = cg; 
                break;
            }
        ASSERT(compReg != nullptr, "Trying to add unknown component with id $0", id);

        // Return component
        return reinterpret_cast<Component*>(e->components[compReg->getIndex()]);
    }

    std::vector<Component*> ComponentManager::getEntityComponentsImpl(EntityId entity)
    {
        DASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");

        EntityBlock* e = getEntityBlock(entity);
        ASSERT(e != nullptr, "Trying to remove component from entity [w]$0[] that was not created", entity);

        std::vector<Component*> components;
        for(size_t i = 0; i < sizeof(EntityBlock)/sizeof(void*); i++)
            components.push_back(reinterpret_cast<Component*>(e->components[i]));

        return components;
    }

    //----------------------------------------//
    //----------------- Views ----------------//
    //----------------------------------------//
    std::vector<EntityId> ComponentManager::getEntitiesView() { return getInstance().getEntitiesViewImpl(); }
    std::vector<EntityId> ComponentManager::getEntitiesViewImpl()
    {
        return std::vector<EntityId>(_entities.begin(), _entities.end());
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
    //----------- Memory Management ----------//
    //----------------------------------------//
    ComponentManager::EntityBlock* ComponentManager::getEntityBlock(EntityId eid)
    {
        BitmapAllocator* epool = MemoryManager::getAllocator<BitmapAllocator>(SID("Component_EntityAllocator"));
        return epool->getBlock<EntityBlock>(eid);
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
