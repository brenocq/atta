//--------------------------------------------------
// Atta Component System
// componentManager.inl
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
    // TODO create multiple entities sequentially in memory (their components must match and be sequential in memory too)
    // entity group?

    template <typename T>
    T* ComponentManager::addEntityComponentImpl(EntityId entity)
    {
        DASSERT(entity < _maxEntities, "Trying to access entity outside of range");
        // TODO Check if entity was created, if this entity was not created, this will break the pool allocator

        // Get entity
        PoolAllocator<Entity>* epool = MemoryManager::getAllocator<PoolAllocator<Entity>>(SID("Component_EntityAllocator"));
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
            LOG_WARN("ComponentManager", "Could not add component $0 to entity $1", typeid(T).name(), entity);
            return nullptr;
        }

        // Alloc component
        PoolAllocator<T>* cpool = MemoryManager::getAllocator<PoolAllocator<T>>(COMPONENT_POOL_SID(T));
        T* component = cpool->alloc();
        *component = T{};

        // Add component to entity
        e->components[freeComponentSlot] = reinterpret_cast<void*>(component);

        return component;
    }

    template <typename T>
    T* ComponentManager::getEntityComponentImpl(EntityId entity)
    {
        DASSERT(entity < _maxEntities, "Trying to access entity outside of range");
        // TODO Check if entity was created, if this entity was not created, this will break the pool allocator

        // Get entity
        PoolAllocator<Entity>* epool = MemoryManager::getAllocator<PoolAllocator<Entity>>(SID("Component_EntityAllocator"));
        Entity* e = epool->getBlock(entity);

        // Get component pool
        PoolAllocator<T>* cpool = MemoryManager::getAllocator<PoolAllocator<T>>(COMPONENT_POOL_SID(T));

        for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
            if(cpool->owns(e->components[i]))
                return reinterpret_cast<T*>(e->components[i]);
        return nullptr;
    }

    template <ComponentId id>
    void* ComponentManager::getEntityComponentByIdImpl(EntityId entity)
    {
        DASSERT(entity < _maxEntities, "Trying to access entity outside of range");
        // TODO Check if entity was created, if this entity was not created, this will break the pool allocator

        // Get entity
        PoolAllocator<Entity>* epool = MemoryManager::getAllocator<PoolAllocator<Entity>>(SID("Component_EntityAllocator"));
        Entity* e = epool->getBlock(entity);

        // Get component pool
        Allocator* alloc = MemoryManager::getAllocator(id);

        for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
            if(alloc->owns(e->components[i]))
                return e->components[i];
        return nullptr;
    }

    template <typename T>
    std::string ComponentManager::getComponentNameImpl()
    {
        if(_componentNames.find(typeid(T).hash_code()) == _componentNames.end())
            return std::string("Unknown");
        return _componentNames[typeid(T).hash_code()];
    }

    template <typename T>
    void ComponentManager::registerComponentPoolImpl(size_t maxCount)
    {
        _registeredComponents.push_back(std::static_pointer_cast<Component>(std::make_shared<T>()));
        std::string name = _registeredComponents.back()->getTypeDescription().type.getString();

        // TODO better pool allocator allocation from another one (now need to know that implementation to implement it correctly)
        // Should not need to calculate this size
        size_t size = sizeof(void*) > sizeof(T) ? sizeof(void*) : sizeof(T);

        uint8_t* componentMemory = reinterpret_cast<uint8_t*>(_allocator->allocBytes(maxCount*size, size));
        DASSERT(componentMemory != nullptr, "Could not allocate component system memory for " + name);
        LOG_INFO("Component Manager", "Allocated memory for component $0 ($1). $2MB -> memory space:($3 $4)", 
                name, typeid(T).name(), maxCount*sizeof(T)/(1024*1024.0f), (void*)(componentMemory), (void*)(componentMemory+maxCount*sizeof(T)));

        // Create pool allocator
        MemoryManager::registerAllocator(COMPONENT_POOL_SSID(T), static_cast<Allocator*>(new PoolAllocator<T>(componentMemory, maxCount)));

        // Register component name and id
        _componentNames[typeid(T).hash_code()] = name;
        _componentIds[typeid(T).hash_code()] = COMPONENT_POOL_SID(T);
        _componentSize[typeid(T).hash_code()] = sizeof(T);
    }
}
