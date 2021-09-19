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
#include <cstring>

namespace atta
{
	void ComponentManager::startUpImpl()
	{
		_maxEntities = 1024*1024;

		//----- System Memory -----//
		// Get main memory
		Allocator* mainAllocator = MemoryManager::getAllocator(SSID("MainAllocator"));
		size_t size = 1024*1024*1024;// 1GB
		// Alloc memory inside main memory
		uint8_t* componentMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
		ASSERT(componentMemory != nullptr, "Could not allocate component system memory");

		// Create new allocator from component memory
		_allocator = new StackAllocator(componentMemory, size);
		MemoryManager::registerAllocator(SSID("ComponentAllocator"), static_cast<Allocator*>(_allocator));

		createEntityPool();
		createComponentPools();

		EventManager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(ComponentManager::onSimulationStateChange));
		EventManager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(ComponentManager::onSimulationStateChange));
	}

	void ComponentManager::shutDownImpl()
	{

	}

	void ComponentManager::createEntityPool()
	{
		const size_t entityMemorySize = (sizeof(EntityId)+sizeof(Entity))*_maxEntities;// TODO Entity objects may need to me aligned

		LOG_VERBOSE("ComponentManager", "Allocating memory for entities. $0MB\n - Entity block size: $1.\n - Limits: \n    - $2 entities\n    - $3 components per entity", entityMemorySize/(1024*1024.0f), sizeof(EntityId), _maxEntities, sizeof(Entity)/sizeof(void*));

		// Allocate from component system memory
		uint8_t* entityMemory = reinterpret_cast<uint8_t*>(_allocator->allocBytes(entityMemorySize, sizeof(Entity)));
		ASSERT(entityMemory != nullptr, "Could not allocate component system entity memory");

		// Use start of the allocate memory for the dense list
		_denseList = reinterpret_cast<EntityId*>(entityMemory);
		_denseListSize = 0;
		
		// Create entity pool allocator
		uint8_t* startEntityPool = entityMemory+sizeof(EntityId)*_maxEntities;
		MemoryManager::registerAllocator(SSID("Component_EntityAllocator"), 
				static_cast<Allocator*>(new PoolAllocator<Entity>(startEntityPool, _maxEntities)));
	}

	void ComponentManager::createComponentPools()
	{
		//----- Register default component pools -----//
		registerComponentPoolImpl<TransformComponent>(_maxEntities, "Transform");
		registerComponentPoolImpl<NameComponent>(_maxEntities, "Name");
		registerComponentPoolImpl<MeshComponent>(_maxEntities, "Mesh");
		registerComponentPoolImpl<ScriptComponent>(_maxEntities, "Script");
		registerComponentPoolImpl<PrototypeComponent>(_maxEntities, "Prototype");
		registerComponentPoolImpl<MaterialComponent>(_maxEntities, "Material");
		registerComponentPoolImpl<PointLightComponent>(_maxEntities, "Point Light");
		registerComponentPoolImpl<DirectionalLightComponent>(_maxEntities, "Directional Light");

		// Can be used to free all custom component allocators (useful when hot-reloading a project)
		_customComponentsMarker = _allocator->getMarker();
	}

	EntityId ComponentManager::createEntityImpl()
	{
		PoolAllocator<Entity>* pool = MemoryManager::getAllocator<PoolAllocator<Entity>>(SID("Component_EntityAllocator"));

		// Alloc entity
		Entity* e = pool->alloc();

		// Initialize entity component pointers
		for(size_t i = 0; i < sizeof(Entity)/sizeof(void*); i++)
			e->components[i] = nullptr;

		// Calculate entityId (index inside pool memory)
		EntityId eid = static_cast<EntityId>(pool->getIndex(e));

		// Add entity to dense list
		_denseList[_denseListSize++] = eid;

		return eid;
	}

	std::vector<std::string> ComponentManager::getComponentNamesImpl()
	{
		std::vector<std::string> componentNames;
		for(auto p : _componentNames)
			componentNames.push_back(p.second);

		return componentNames;
	}

	std::vector<EntityId> ComponentManager::getEntitiesImpl()
	{
		return std::vector<EntityId>(_denseList, _denseList+_denseListSize);
	}

	void ComponentManager::clearImpl()
	{
		PoolAllocator<Entity>* epool = MemoryManager::getAllocator<PoolAllocator<Entity>>(SID("Component_EntityAllocator"));
		epool->clear();

		_denseListSize = 0;

		PoolAllocator<NameComponent>* namePool = MemoryManager::getAllocator<PoolAllocator<NameComponent>>(COMPONENT_POOL_SID(NameComponent));
		namePool->clear();

		PoolAllocator<MeshComponent>* meshPool = MemoryManager::getAllocator<PoolAllocator<MeshComponent>>(COMPONENT_POOL_SID(MeshComponent));
		meshPool->clear();

		PoolAllocator<ScriptComponent>* scriptPool = MemoryManager::getAllocator<PoolAllocator<ScriptComponent>>(COMPONENT_POOL_SID(ScriptComponent));
		scriptPool->clear();

		PoolAllocator<TransformComponent>* transformPool = MemoryManager::getAllocator<PoolAllocator<TransformComponent>>(COMPONENT_POOL_SID(TransformComponent));
		transformPool->clear();
	}

	void ComponentManager::unregisterCustomComponentPoolsImpl()
	{
		_allocator->rollback(_customComponentsMarker);
	}

	//---------- Factory Management ----------//
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

	void ComponentManager::createFactories()
	{
		LOG_WARN("ComponentManager", "Create factories");
		// Create factory for each entity that has prototype
		for(auto entity : getEntities())
		{
			PrototypeComponent* prototype = getEntityComponentImpl<PrototypeComponent>(entity);
			if(prototype != nullptr)
			{
				std::vector<std::pair<size_t, uint8_t*>> componentMemories;

				LOG_WARN("ComponentManager", "Entity $0 is an prototype", entity);
				// Allocate memory for each 
				for(auto [componentHash, componentId] : _componentIds)	
				{
					// Get component pool
					Allocator* alloc = MemoryManager::getAllocator(componentId);
					size_t componentSize = _componentSize[componentHash];

					if(prototype->maxClones > 0)
					{
						uint8_t* mem = (uint8_t*)alloc->allocBytes(componentSize, componentSize);
						componentMemories.push_back(std::make_pair(componentHash, mem));
						LOG_WARN("ComponentManager", "Found component $0, mem:$1", _componentNames[componentHash], (void*)mem);
					}

					for(uint32_t i = 1; i < prototype->maxClones; i++)
						alloc->allocBytes(componentSize, componentSize);
				}

				Factory::CreateInfo info {};
				info.prototypeId = entity;
				info.maxClones = prototype->maxClones;
				info.componentMemories = componentMemories;
				_factories.emplace_back(info);
			}
		}
	}

	void ComponentManager::destroyFactories()
	{ 
		LOG_WARN("ComponentManager", "Destroy factories");
		// Release memory for each factory
		// Because we are using a pool free list allocator, need to delete from last to first element
		for(int i = _factories.size()-1; i>=0; i--)
		{
			//LOG_WARN("ComponentManager", "Destroying factory $0", i);
			auto maxClones = _factories[i].getMaxClones();
			for(auto [componentHash, memory] : _factories[i].getComponentMemories())
			{
				//LOG_WARN("ComponentManager", "Deleting component $0, mem:$1", _componentNames[componentHash], (void*)memory);
				// Delete from last to first
				size_t componentSize = _componentSize[componentHash];
				size_t blockSize = sizeof(void*) > componentSize ? sizeof(void*) : componentSize;
				ComponentId componentId = _componentIds[componentHash];
				Allocator* alloc = MemoryManager::getAllocator(componentId);

				for(uint8_t* it = memory+blockSize*maxClones; it >= memory; it-=blockSize)
					alloc->freeBytes(it, componentSize, componentSize);
			}
		}
		_factories.clear();
	}
}
