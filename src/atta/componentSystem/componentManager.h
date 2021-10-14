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
#include <atta/eventSystem/eventManager.h>
#include <atta/componentSystem/base.h>
#include <atta/componentSystem/factory.h>

namespace atta
{
#define COMPONENT_POOL_SID(T) SID((std::string("Component_") + typeid(T).name() + "Allocator").c_str())
#define COMPONENT_POOL_SSID(T) SSID((std::string("Component_") + typeid(T).name() + "Allocator").c_str())

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
        template <ComponentId id>
        static void* getEntityComponentById(EntityId entity) { return getInstance().getEntityComponentByIdImpl<id>(entity); }

        template <typename T>
        static std::string getComponentName() { return getInstance().getComponentNameImpl<T>(); }
        static std::vector<std::string> getComponentNames() { return getInstance().getComponentNamesImpl(); }

        static std::vector<EntityId> getEntities() { return getInstance().getEntitiesImpl(); }
        static std::vector<Factory>& getFactories() { return getInstance().getFactoriesImpl(); }

        template <typename T>
        static void registerComponentPool(size_t maxCount, const char* name) { return getInstance().registerComponentPoolImpl<T>(maxCount, name); }

        static void clear() { getInstance().clearImpl(); }

        // Return stack pointer to the point before custom components (free custom component allocators)
        static void unregisterCustomComponentPools() { getInstance().unregisterCustomComponentPoolsImpl(); }

    private:
        //----- Startup/ShutDown -----//
        void startUpImpl();
        void shutDownImpl();
        void clearImpl();
        void createEntityPool();
        void createComponentPools();

        //----- Public Interface -----//
        EntityId createEntityImpl();
        template <typename T>
        T* addEntityComponentImpl(EntityId entity);
        template <typename T>
        T* getEntityComponentImpl(EntityId entity);
        template <ComponentId id>
        void* getEntityComponentByIdImpl(EntityId entity);
        template <typename T>
        std::string getComponentNameImpl();
        std::vector<std::string> getComponentNamesImpl();
        std::vector<EntityId> getEntitiesImpl();
        std::vector<Factory>& getFactoriesImpl() { return _factories; }

        // Used to register internal components and custom components
        template <typename T>
        void registerComponentPoolImpl(size_t maxCount, const char* name);
        // Unregister to free memory that was allocated for all custom components
        void unregisterCustomComponentPoolsImpl();

        //----- Memory management -----//
        struct Entity
        {
            void* components[8];
        };
        StackAllocator* _allocator;// Used to allocate more memory to component pools
        StackAllocator::Marker _customComponentsMarker;// Marker to free custom components

        size_t _maxEntities;// Maximum number of entities
        EntityId* _denseList;// Dense list of entities to find active entities inside the entity pool
        size_t _denseListSize;
        std::unordered_map<size_t, std::string> _componentNames;// Name of each registered component
        std::unordered_map<size_t, ComponentId> _componentIds;// id of each registered component
        std::unordered_map<size_t, uint32_t> _componentSize;// sizeof(T) of each registered component

        //----- Factory Management -----//
        void onSimulationStateChange(Event& event);
        void createFactories();
        void destroyFactories();

        std::vector<Factory> _factories;
    };
}

#include <atta/componentSystem/componentManager.inl>
#endif// ATTA_COMPONENT_SYSTEM_COMPONENT_MANAGER_H
