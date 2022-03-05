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
#include <atta/eventSystem/events/createEntityEvent.h>
#include <atta/eventSystem/events/createComponentEvent.h>
#include <atta/eventSystem/events/deleteEntityEvent.h>
#include <atta/eventSystem/events/deleteComponentEvent.h>
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

        //LOG_VERBOSE("ComponentManager", "Allocating memory for entities. $0MB\n - Entity block size: $1.\n - Limits: \n    - $2 entities\n    - $3 components per entity", entityMemorySize/(1024*1024.0f), sizeof(EntityId), _maxEntities, sizeof(EntityBlock)/sizeof(void*));

        // Allocate from component system memory
        uint8_t* entityMemory = reinterpret_cast<uint8_t*>(_allocator->allocBytes(entityMemorySize, sizeof(EntityBlock)));// TODO Probably not aligned because of the bitmap 
        ASSERT(entityMemory != nullptr, "Could not allocate component system entity memory");

        // Create entity bitmap pool allocator
        MemoryManager::registerAllocator(SSID("Component_EntityAllocator"), 
                static_cast<Allocator*>(new BitmapAllocator(entityMemory, entityMemorySize, sizeof(EntityBlock))));
    }

    EntityId ComponentManager::createEntity(EntityId entity) { return getInstance().createEntityImpl(entity); }
    EntityId ComponentManager::createEntityImpl(EntityId entity, size_t quantity)
    {
        BitmapAllocator* pool = MemoryManager::getAllocator<BitmapAllocator>(SID("Component_EntityAllocator"));

        if(entity != -1 && pool->getBlockBit(entity))
        {
            LOG_WARN("ComponentManager", "Trying to create entity [w]$0[] that already exists", entity);
            return -1;
        }

        // Alloc entity
        EntityBlock* e = nullptr;
        if(entity == -1)
            e = pool->alloc<EntityBlock>(quantity);
        else
            e = pool->allocAtIndex<EntityBlock>(entity, quantity);
        ASSERT(e != nullptr, "Could not create entity, probably atta memory is full");

        // Initialize entity component pointers
        for(EntityBlock* it = e; it < e+quantity; it++)
            for(size_t i = 0; i < sizeof(EntityBlock)/sizeof(void*); i++)
                it->components[i] = nullptr;

        // Calculate entityId (index inside pool memory)
        EntityId eid = static_cast<EntityId>(pool->getIndex(e));

        for(EntityId i = eid; i < EntityId(eid+quantity); i++)
        {
            _noPrototypeView.insert(i);
            _entities.insert(i);
        }

        // Publish create entity event
        CreateEntityEvent event;
        event.entityId = eid;
        EventManager::publish(event);

        return eid;
    }

    EntityId ComponentManager::createClones(size_t quantity) { return getInstance().createClonesImpl(quantity); }
    EntityId ComponentManager::createClonesImpl(size_t quantity)
    {
        EntityId eid = createEntityImpl(-1, quantity);
        for(EntityId i = eid; i < eid+EntityId(quantity); i++)
            _cloneView.insert(i);
        return eid;
    }

    void ComponentManager::deleteEntity(EntityId entity) { getInstance().deleteEntityImpl(entity); }
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

        // Publish delete entity event
        DeleteEntityEvent event;
        event.entityId = entity;
        EventManager::publish(event);
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

        // Publish delete entity event
        DeleteEntityEvent event;
        event.entityId = entity;
        EventManager::publish(event);
    }

    EntityId ComponentManager::copyEntity(EntityId entity) { return getInstance().copyEntityImpl(entity); }
    EntityId ComponentManager::copyEntityImpl(EntityId entity)
    {
        // Get entity
        EntityBlock* e = getEntityBlock(entity);
        ASSERT(e != nullptr, "Trying to copy entity [w]$0[] that does not exists", entity);

        EntityId newEntity = createEntity();

        // Delete allocated components
        for(auto compReg : _componentRegistries) 
        {
            // If base entity has this component
            Component* baseComponent = getEntityComponentById(compReg->getId(), entity);
            if(baseComponent)
            {
                Component* component = addEntityComponentById(compReg->getId(), newEntity);

                // TODO implement component copy operator
                if(compReg->getId() != TypedComponentRegistry<RelationshipComponent>::getInstance().getId())
                    memcpy(component, baseComponent, compReg->getSizeof());
                else
                {
                    RelationshipComponent* baseR = static_cast<RelationshipComponent*>(baseComponent);
                    RelationshipComponent* copyR = static_cast<RelationshipComponent*>(component);
                    if(baseR->getParent() != -1)
                        copyR->setParent(baseR->getParent(), newEntity);
                }
            }
        }

        return newEntity;
    }

    void ComponentManager::clearImpl()
    {
        // Clear entities
        BitmapAllocator* epool = MemoryManager::getAllocator<BitmapAllocator>(SID("Component_EntityAllocator"));
        epool->clear();

        // Clear entity view
        _entities.clear();
        _noPrototypeView.clear();
        _cloneView.clear();
        _scriptView.clear();

        // Clear components
        for(auto reg : _componentRegistries)
            getComponentAllocator(reg)->clear();
    }

    BitmapAllocator* ComponentManager::getComponentAllocator(ComponentRegistry* compReg)
    {
        return MemoryManager::getAllocator<BitmapAllocator>(COMPONENT_POOL_SID_BY_NAME(compReg->getTypeidName()));
    }

    void ComponentManager::registerComponentImpl(ComponentRegistry* componentRegistry)
    {
        ASSERT(_componentRegistries.size() < maxRegisteredComponents, "More components than it is possible to store inside the entityBlock, maximum is $0", maxRegisteredComponents);

        // Check if not already registered
        int oldIndex = -1;
        for(unsigned i = 0; i < _componentRegistries.size() && i < _componentRegistriesBackupInfo.size(); i++)
            if(componentRegistry->getTypeidHash() == _componentRegistriesBackupInfo[i].typeidHash)
            {
                oldIndex = i;
                break;
            }

        if(oldIndex == -1)
        {
            LOG_DEBUG("ComponentManager", "Registered component [w]$0[]", componentRegistry->getTypeidName());
            componentRegistry->setIndex(_componentRegistries.size());
            _componentRegistries.push_back(componentRegistry);

            // Push new to registered backup (will be updated later because the Description data may not be available while the components are being registered)
            // Need to keep track of this because registerComponentImpl can be called multiple times for the same component (one time for each translation unit). We need to be sure that will not push the same componentRegistry twice
            _componentRegistriesBackupInfo.push_back({componentRegistry->getTypeidHash(), ComponentDescription{componentRegistry->getTypeidName()}, false});
        }
        else
        {
            // XXX Not removing old components
            //LOG_DEBUG("ComponentManager", "Reloading component [w]$0[]", componentRegistry->getTypeidName());
            // If the layout changed: (TODO)
            //   1. Save entities with old data
            //   2. Deallocate old memory
            //   3. Allocate new memory
            //   4. Change entity component pointer and copy+format component data
            // If the layout was not changed:
            //   No nothing
            if(_componentRegistries[oldIndex] != componentRegistry)
            {
                //LOG_WARN("ComponentManager", "Component registry pointer changed from $0 to $1\n new: $0", _componentRegistries[oldIndex], componentRegistry);
                _componentRegistries[oldIndex] = componentRegistry;
            }

            componentRegistry->setPoolCreated(_componentRegistriesBackupInfo[oldIndex].poolCreated);
            componentRegistry->setIndex(oldIndex);
        }
    }

    void ComponentManager::createComponentPoolsFromRegistered()
    {
        for(auto reg : _componentRegistries)
        {
            // TODO Remove custom component registry when it is not loaded (pointer to random data)
            if(!reg->getPoolCreated())
            {
                //LOG_DEBUG("ComponentManager", "Create pool [w]$0[] -> $1", reg->getTypeidName(), reg->getPoolCreated());
                LOG_DEBUG("ComponentManager", "Create component pool [w]$0[]", reg->getDescription().name);
                createComponentPool(reg);
                reg->setPoolCreated(true);
            }
        }
    }

    void ComponentManager::createComponentPool(ComponentRegistry* componentRegistry)
    {
        ComponentDescription& desc = componentRegistry->getDescription();
        std::string name = desc.name;
        unsigned sizeofT = componentRegistry->getSizeof();
        std::string typeidTName = componentRegistry->getTypeidName();
        unsigned maxCount = desc.maxInstances;

        // TODO better bitmap allocator allocation from another allocator (now need to know that implementation to implement it correctly)
        // Should not need to calculate this size
        size_t size = ceil(maxCount/8.0f) + sizeofT*maxCount;

        uint8_t* componentMemory = reinterpret_cast<uint8_t*>(_allocator->allocBytes(size, sizeofT));
        DASSERT(componentMemory != nullptr, "Could not allocate component system memory for " + name);
        LOG_INFO("Component Manager", "Allocated memory for component $0 ($1). $2MB ($5 instances) -> memory space:($3 $4)", 
                name, typeidTName, maxCount*sizeofT/(1024*1024.0f), (void*)(componentMemory), (void*)(componentMemory+maxCount*sizeofT), maxCount);

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
        ASSERT(e != nullptr, "Trying to add component [w]$0[] to entity [w]$1[] that was not created", compReg->getDescription().name, entity);

        if(e->components[compReg->getIndex()] != nullptr)
        {
            LOG_WARN("ComponentManager", "Could not add component [w]$1[] to entity [w]$0[]. The entity [w]$0[] already has the component [w]$1[]", entity, compReg->getDescription().name);
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

            // Publish create component event
            CreateComponentEvent event;
            event.componentId = id;
            event.entityId = entity;
            event.component = component;
            EventManager::publish(event);

            return component;
        }
        else
        {
            LOG_WARN("ComponentManager", "Could not allocate component $0 for entity $1", compReg->getDescription().name, entity);
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

        ComponentId id = _componentRegistries[index]->getId();

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

        Component* componentPtr = reinterpret_cast<Component*>(e->components[index]);

        // Publish create component event
        CreateComponentEvent event;
        event.componentId = id;
        event.entityId = entity;
        event.component = componentPtr;
        EventManager::publish(event);

        return componentPtr;
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

        // Publish delete component event
        DeleteComponentEvent event;
        event.componentId = id;
        event.entityId = entity;
        EventManager::publish(event);
    }

    //----------------------------------------//
    //--------- Get entity component ---------//
    //----------------------------------------//
    Component* ComponentManager::getEntityComponentByIdImpl(ComponentId id, EntityId entity)
    {
        // Get component registry
        ComponentRegistry* compReg = nullptr;
        for(auto cg : _componentRegistries)
            if(cg->getId() == id)
            {
                compReg = cg; 
                break;
            }
        ASSERT(compReg != nullptr, "Trying to add unknown component with id $0", id);

		return getEntityComponentByIndex(compReg->getIndex(), entity);
    }

    Component* ComponentManager::getEntityComponentByIndex(unsigned index, EntityId entity)
	{
        DASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");
        DASSERT(index < maxRegisteredComponents, "Trying to access component by index outside of range");

        // Get entity
        EntityBlock* e = getEntityBlock(entity);
        ASSERT(e != nullptr, "Trying to get component [w]$1[] from entity [w]$0[], but this entity was not created", entity, _componentRegistries[index]->getDescription().name);

        // Return component
        return reinterpret_cast<Component*>(e->components[index]);
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

    std::vector<EntityId> ComponentManager::getNoCloneView() { return getInstance().getNoCloneViewImpl(); }
    std::vector<EntityId> ComponentManager::getNoCloneViewImpl()
    {
        std::vector<EntityId> noClones;
        for(auto entity : getEntitiesView())
            if(_cloneView.find(entity) == _cloneView.end())
                noClones.push_back(entity);
        return noClones;
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

    Factory* ComponentManager::getPrototypeFactoryImpl(EntityId prototype)
    {
        for(Factory& factory : _factories)
            if(factory.getPrototypeId() == prototype)
                return &factory;
        LOG_WARN("ComponentManager", "Trying to get factory from entity [w]$0[] that is not a prototype", prototype);
        return nullptr;
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
					bool found = false;
					for(auto op : TypedComponentRegistry<MeshComponent>::description->attributeDescriptions[0].options)
						if(std::any_cast<StringId>(op) == e.sid)
						{
							found = true;
							break;
						}

					if(!found)
                    	TypedComponentRegistry<MeshComponent>::description->attributeDescriptions[0].options.push_back(std::any(e.sid));

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

                    // Update material options
                    {
                        bool found = false;
                        for(auto op : TypedComponentRegistry<MaterialComponent>::description->attributeDescriptions[4].options)
                            if(std::any_cast<StringId>(op) == e.sid)
                            {
                                found = true;
                                break;
                            }

                        if(!found)
                        {
                            TypedComponentRegistry<MaterialComponent>::description->attributeDescriptions[4].options.push_back(std::any(e.sid));
                            TypedComponentRegistry<MaterialComponent>::description->attributeDescriptions[5].options.push_back(std::any(e.sid));
                            TypedComponentRegistry<MaterialComponent>::description->attributeDescriptions[6].options.push_back(std::any(e.sid));
                            TypedComponentRegistry<MaterialComponent>::description->attributeDescriptions[8].options.push_back(std::any(e.sid));
                            TypedComponentRegistry<MaterialComponent>::description->attributeDescriptions[7].options.push_back(std::any(e.sid));
                        }
                    }

                    // Update environment light options
                    {
                        bool found = false;
                        for(auto op : TypedComponentRegistry<EnvironmentLightComponent>::description->attributeDescriptions[0].options)
                            if(std::any_cast<StringId>(op) == e.sid)
                            {
                                found = true;
                                break;
                            }

                        if(!found)
                        {
                            TypedComponentRegistry<EnvironmentLightComponent>::description->attributeDescriptions[0].options.push_back(std::any(e.sid));
                        }
                    }

                    break;
                }
            default:
                break;
        }
    }

    void ComponentManager::onScriptEvent(Event& event)
    {
        ScriptTargetEvent& e = reinterpret_cast<ScriptTargetEvent&>(event);

        TypedComponentRegistry<ScriptComponent>::description->attributeDescriptions[0].options.clear();
        for(StringId script : e.scriptSids)
            TypedComponentRegistry<ScriptComponent>::description->attributeDescriptions[0].options.push_back(std::any(script));

        // Created pool to new components if necessary
        createComponentPoolsFromRegistered();

        // Update backup info (only now we can guarantee that the description data is available)
        _componentRegistriesBackupInfo.clear();
        for(auto reg : _componentRegistries)
        {
            //LOG_DEBUG("ComponentManager", "Update backup from $0", reg->getDescription().name);
            ComponentRegistryBackupInfo crbi;
            crbi.typeidHash = reg->getTypeidHash();
            crbi.description.name = reg->getDescription().name;
            crbi.description.attributeDescriptions = reg->getDescription().attributeDescriptions;
            crbi.poolCreated = true;
            _componentRegistriesBackupInfo.push_back(crbi);
        }
    }
}
