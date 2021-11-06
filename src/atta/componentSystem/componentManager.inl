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
        return reinterpret_cast<T*>(addEntityComponentByIdImpl(COMPONENT_POOL_SSID(T), entity));
    }

    template <typename T>
    T* ComponentManager::getEntityComponentImpl(EntityId entity)
    {
        DASSERT(entity < (int)_maxEntities, "Trying to access entity outside of range");
        // TODO Check if entity was created, if this entity was not created, this will break the pool allocator

        // Get entity
        PoolAllocatorT<Entity>* epool = MemoryManager::getAllocator<PoolAllocatorT<Entity>>(SID("Component_EntityAllocator"));
        Entity* e = epool->getBlock(entity);

        // Get component pool
        PoolAllocatorT<T>* cpool = MemoryManager::getAllocator<PoolAllocatorT<T>>(COMPONENT_POOL_SID(T));

        for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
            if(cpool->owns(e->components[i]))
                return reinterpret_cast<T*>(e->components[i]);
        return nullptr;
    }

    template <typename T>
    std::string ComponentManager::getComponentNameImpl()
    {
        if(_componentNames.find(typeid(T).hash_code()) == _componentNames.end())
            return std::string("Unknown");
        return _componentNames[typeid(T).hash_code()];
    }
}
