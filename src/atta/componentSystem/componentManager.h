//--------------------------------------------------
// Atta Component System
// componentManager.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENT_MANAGER_H
#define ATTA_COMPONENT_SYSTEM_COMPONENT_MANAGER_H
#include <atta/memorySystem/allocators/stackAllocator.h>
#include <atta/memorySystem/allocators/poolAllocatorT.h>
#include <atta/memorySystem/memoryManager.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/componentSystem/base.h>
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/factory.h>

namespace atta
{
#define COMPONENT_POOL_SID(T) SID((std::string("Component_") + typeid(T).name() + "Allocator").c_str())
#define COMPONENT_POOL_SSID(T) SSID((std::string("Component_") + typeid(T).name() + "Allocator").c_str())
#define COMPONENT_POOL_SID_BY_NAME(typeidTname) SID((std::string("Component_") + typeidTname + "Allocator").c_str())
#define COMPONENT_POOL_SSID_BY_NAME(typeidTname) SSID((std::string("Component_") + typeidTname + "Allocator").c_str())

    class ComponentRegistry;
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

        // Create/destroy entity
        static EntityId createEntity();
        static void destroyEntity(EntityId entity);// Destroy entity and deallocate components
        static void destroyEntityOnly(EntityId entity);// Destroy entity without deallocating components

        // Add entity component
        template <typename T>
        static T* addEntityComponent(EntityId entity) { return getInstance().addEntityComponentImpl<T>(entity); }
        static Component* addEntityComponentById(ComponentId id, EntityId entity) { return getInstance().addEntityComponentByIdImpl(id, entity); }
        static Component* addEntityComponentPtr(EntityId entity, uint8_t* component) { return getInstance().addEntityComponentPtrImpl(entity, component); }
        // Get entity component
        template <typename T>
        static T* getEntityComponent(EntityId entity) { return getInstance().getEntityComponentImpl<T>(entity); }
        static Component* getEntityComponentById(ComponentId id, EntityId entity) { return getInstance().getEntityComponentByIdImpl(id, entity); }
        static std::vector<Component*> getEntityComponents(EntityId entity) { return getInstance().getEntityComponentsImpl(entity); }
        // Remove entity component
        static void removeEntityComponentById(ComponentId id, EntityId entity) { getInstance().removeEntityComponentByIdImpl(id, entity); }

        // Getters
        static std::vector<ComponentRegistry*> getComponentRegistries() { return getInstance().getComponentRegistriesImpl(); }
        static std::vector<Factory>& getFactories() { return getInstance().getFactoriesImpl(); }

        // Views
        static std::vector<EntityId> getEntitiesView();
        static std::vector<EntityId> getNoPrototypeView();
        static std::vector<EntityId> getCloneView();
        static std::vector<EntityId> getScriptView();

        // Memory management
        static void createDefault() { getInstance().createDefaultImpl(); }
        static void clear() { getInstance().clearImpl(); }
        static void registerComponent(ComponentRegistry* componentRegistry) { return getInstance().registerComponentImpl(componentRegistry); }
        static void unregisterCustomComponents() { getInstance().unregisterCustomComponentsImpl(); }

    private:
        //----- Startup/ShutDown -----//
        void startUpImpl();
        void shutDownImpl();
        void createDefaultImpl();
        void clearImpl();
        void createEntityPool();

        //----- Public Interface -----//
        static EntityId createClone();
        EntityId createCloneImpl();
        EntityId createEntityImpl();
        void destroyEntityImpl(EntityId entity);
        void destroyEntityOnlyImpl(EntityId entity);

        template <typename T>
        T* addEntityComponentImpl(EntityId entity);
        Component* addEntityComponentByIdImpl(ComponentId id, EntityId entity);
        Component* addEntityComponentPtrImpl(EntityId entity, uint8_t* component);
        void removeEntityComponentByIdImpl(ComponentId id, EntityId entity);
        template <typename T>
        T* getEntityComponentImpl(EntityId entity);
        Component* getEntityComponentByIdImpl(ComponentId id, EntityId entity);
        std::vector<Component*> getEntityComponentsImpl(EntityId entity);

        std::vector<Factory>& getFactoriesImpl() { return _factories; }
        std::vector<ComponentRegistry*> getComponentRegistriesImpl() { return _componentRegistries; }
        PoolAllocator* getComponentAllocator(ComponentRegistry* compReg);

        // Views
        std::vector<EntityId> getEntitiesViewImpl();
        std::vector<EntityId> getNoPrototypeViewImpl();
        std::vector<EntityId> getCloneViewImpl();
        std::vector<EntityId> getScriptViewImpl();

        // Used to register internal components and custom components
        void registerComponentImpl(ComponentRegistry* componentRegistry);
        void createComponentPoolsFromRegistered();
        void createComponentPool(ComponentRegistry* componentRegistry);
        // Unregister to free memory that was allocated for all custom components
        void unregisterCustomComponentsImpl();

        //----- Event handling -----//
        void onMeshEvent(Event& event);// Used the update the MeshComponent attribute options
        void onTextureEvent(Event& event);// Used the update the MaterialComponent attribute options
        void onScriptEvent(Event& event);// Used the update the ScriptComponent attribute options

        //----- Memory management -----//
        struct Entity
        {
            void* components[8];
        };
        StackAllocator* _allocator;// Used to allocate more memory to component pools
        StackAllocator::Marker _customComponentsMarker;// Marker to free custom components
        size_t _numAttaComponents;// Used to remove custom components form componentRegistries
        std::vector<ComponentRegistry*> _componentRegistries;// All registered components

        // Dense list of all entities
        size_t _maxEntities;// Maximum number of entities
        EntityId* _denseList;// Dense list of entities to find active entities inside the entity pool
        size_t _denseListSize;

        // Entity views(TODO create views from template?)
        std::set<EntityId> _noPrototypeView;// View of entities and clone (no prototype entity)
        std::set<EntityId> _cloneView;// View of only clones
        std::set<EntityId> _scriptView;// View of entities that are neither prototype or clone and have script component

        //----- Factory Management -----//
        void onSimulationStateChange(Event& event);
        void createFactories();
        void destroyFactories();

        std::vector<Factory> _factories;

        friend Factory;
    };
}

#include <atta/componentSystem/componentManager.inl>
#endif// ATTA_COMPONENT_SYSTEM_COMPONENT_MANAGER_H
