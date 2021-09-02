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
		PoolAllocator<Entity>* epool = MemoryManager::getAllocator<PoolAllocator<Entity>>(SID("ComponentSystem_Entity"));
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
			return nullptr;

		// Alloc component
		PoolAllocator<T>* cpool = MemoryManager::getAllocator<PoolAllocator<T>>(COMPONENT_POOL_ID(T));
		T* component = cpool->alloc();

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
		PoolAllocator<Entity>* epool = MemoryManager::getAllocator<PoolAllocator<Entity>>(SID("ComponentSystem_Entity"));
		Entity* e = epool->getBlock(entity);

		// Get component pool
		PoolAllocator<T>* cpool = MemoryManager::getAllocator<PoolAllocator<T>>(COMPONENT_POOL_ID(T));

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

	template <typename T>
	void ComponentManager::registerComponentPoolImpl(size_t maxCount, const char* name)
	{
		uint8_t* componentMemory = reinterpret_cast<uint8_t*>(_allocator->alloc<T>(maxCount));
		DASSERT(componentMemory != nullptr, std::string("Could not allocate component system memory for ") + std::string(name));
		LOG_INFO("Component Manager", "Allocated memory for component $0 ($1). $2MB", name, typeid(T).name(), maxCount*sizeof(T)/(1024*1024.0f));
		
		// Create pool allocator
		MemoryManager::registerAllocator(COMPONENT_POOL_ID(T), static_cast<Allocator*>(new PoolAllocator<T>(componentMemory, maxCount)));

		// Register component name
		_componentNames[typeid(T).hash_code()] = std::string(name);
	}
}
