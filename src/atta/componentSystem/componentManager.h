//--------------------------------------------------
// Atta Component System
// componentManager.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENT_MANAGER_H
#define ATTA_COMPONENT_SYSTEM_COMPONENT_MANAGER_H
#include <atta/memorySystem/allocators/stackAllocator.h>
#include <atta/memorySystem/allocators/poolAllocator.h>
#include <atta/memorySystem/memoryManager.h>
#include <atta/core/stringId.h>

namespace atta
{
#define COMPONENT_POOL_SID(T) SID((std::string("Component_") + typeid(T).name() + "Allocator").c_str())
#define COMPONENT_POOL_SSID(T) SSID((std::string("Component_") + typeid(T).name() + "Allocator").c_str())
	using EntityId = size_t;


	class ComponentManager final
	{
	public:

		static ComponentManager& getInstance()
		{
			static ComponentManager instance;
			return instance;
		}

		static void startUp() { getInstance().startUpImpl(); }
		static void shutDown() { getInstance().shutDownImpl(); }

		static EntityId createEntity() { return getInstance().createEntityImpl(); }
		template <typename T>
		static T* addEntityComponent(EntityId entity) { return getInstance().addEntityComponentImpl<T>(entity); }
		template <typename T>
		static T* getEntityComponent(EntityId entity) { return getInstance().getEntityComponentImpl<T>(entity); }

		template <typename T>
		static std::string getComponentName() { return getInstance().getComponentNameImpl<T>(); }
		static std::vector<std::string> getComponentNames() { return getInstance().getComponentNamesImpl(); }

		static std::vector<EntityId> getEntities() { return getInstance().getEntitiesImpl(); }

		template <typename T>
		static void registerComponentPool(size_t maxCount, const char* name) { return getInstance().registerComponentPoolImpl<T>(maxCount, name); }

		static void clear() { getInstance().clearImpl(); }

	private:
		void startUpImpl();
		void shutDownImpl();
		void createEntityPool();
		void createComponentPools();
		void clearImpl();

		EntityId createEntityImpl();
		template <typename T>
		T* addEntityComponentImpl(EntityId entity);
		template <typename T>
		T* getEntityComponentImpl(EntityId entity);
		template <typename T>
		std::string getComponentNameImpl();
		std::vector<std::string> getComponentNamesImpl();
		std::vector<EntityId> getEntitiesImpl();

		template <typename T>
		void registerComponentPoolImpl(size_t maxCount, const char* name);

		struct Entity
		{
			void* components[8];
		};

		StackAllocator* _allocator;// Used to allocate more memory to component pools
		size_t _maxEntities;// Maximum number of entities
		EntityId* _denseList;// Dense list of entities to find active entities inside the entity pool
		size_t _denseListSize;
		std::unordered_map<size_t, std::string> _componentNames;
	};
}

#include <atta/componentSystem/componentManager.inl>
#endif// ATTA_COMPONENT_SYSTEM_COMPONENT_MANAGER_H
