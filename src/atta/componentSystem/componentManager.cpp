//--------------------------------------------------
// Atta Component System
// componentManager.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/components.h>
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

		EntityId e0 = createEntity();
		EntityId e1 = createEntity();
		EntityId e2 = createEntity();
		EntityId e3 = createEntity();

		TransformComponent* t = addEntityComponent<TransformComponent>(e0);
		t->position = vec3(0.5f, 0.5f, 0.0f);
		t->scale = vec3(0.1f, 0.1f, 0.1f);

		NameComponent* n0 = addEntityComponent<NameComponent>(e0);
		strcpy(n0->name,"e0 object");
		NameComponent* n1 = addEntityComponent<NameComponent>(e1);
		strcpy(n1->name,"e1 object");
		NameComponent* n2 = addEntityComponent<NameComponent>(e2);
		strcpy(n2->name,"e2 object");

		MeshComponent* m0 = addEntityComponent<MeshComponent>(e0);
		m0->sid = StringId(fs::absolute("../resources/meshes/cube.obj").string());

		MeshComponent* m1 = addEntityComponent<MeshComponent>(e1);
		m1->sid = StringId(fs::absolute("../resources/meshes/plane.obj").string());
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
}
