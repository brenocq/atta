//--------------------------------------------------
// Atta Component Module
// componentManager.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENT_MANAGER_H
#define ATTA_COMPONENT_COMPONENT_MANAGER_H
#include <atta/memory/allocators/stackAllocator.h>
#include <atta/memory/allocators/poolAllocatorT.h>
#include <atta/memory/allocators/bitmapAllocator.h>
#include <atta/memory/memoryManager.h>
#include <atta/event/eventManager.h>
#include <atta/component/base.h>
#include <atta/component/typedComponentRegistry.h>
#include <atta/component/components/component.h>
#include <atta/component/factory.h>

namespace atta
{
    constexpr unsigned maxRegisteredComponents = 32;
    constexpr unsigned maxEntities = 1024;

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
        static EntityId createEntity(EntityId entity = -1);// Can try to create entity with specific EntityId
        static void deleteEntity(EntityId entity);// Delete entity and deallocate components
        static void deleteEntityOnly(EntityId entity);// Delete entity without deallocating components
        static EntityId copyEntity(EntityId entity);

        // Add entity component
        template <typename T>
        static T* addEntityComponent(EntityId entity) { return getInstance().addEntityComponentImpl<T>(entity); }
        static Component* addEntityComponentById(ComponentId id, EntityId entity) { return getInstance().addEntityComponentByIdImpl(id, entity); }
        static Component* addEntityComponentPtr(EntityId entity, unsigned index, uint8_t* component) { return getInstance().addEntityComponentPtrImpl(entity, index, component); }
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
        static Factory* getPrototypeFactory(EntityId prototype) { return getInstance().getPrototypeFactoryImpl(prototype); }

        // Views
        static std::vector<EntityId> getEntitiesView();
        static std::vector<EntityId> getNoPrototypeView();
        static std::vector<EntityId> getCloneView();
        static std::vector<EntityId> getNoCloneView();
        static std::vector<EntityId> getScriptView();
        static EntityId getSelectedEntity() { return getInstance()._selectedEntity; }
        static void setSelectedEntity(EntityId eid) { getInstance()._selectedEntity = eid; }

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

        //----- Entity -----//
        EntityId createEntityImpl(EntityId entity = -1, size_t quantity = 1);
        void deleteEntityImpl(EntityId entity);
        void deleteEntityOnlyImpl(EntityId entity);
        EntityId copyEntityImpl(EntityId entity);

        //----- Component -----//
        template <typename T>
        T* addEntityComponentImpl(EntityId entity);
        Component* addEntityComponentByIdImpl(ComponentId id, EntityId entity);
        Component* addEntityComponentPtrImpl(EntityId entity, unsigned index, uint8_t* component);
        void removeEntityComponentByIdImpl(ComponentId id, EntityId entity);
        template <typename T>
        T* getEntityComponentImpl(EntityId entity);
        Component* getEntityComponentByIdImpl(ComponentId id, EntityId entity);
        Component* getEntityComponentByIndex(unsigned index, EntityId entity);
        std::vector<Component*> getEntityComponentsImpl(EntityId entity);

        //----- Views -----//
        // TODO Hardcoded, think some way using templates
        std::vector<EntityId> getEntitiesViewImpl();
        std::vector<EntityId> getNoPrototypeViewImpl();
        std::vector<EntityId> getCloneViewImpl();
        std::vector<EntityId> getNoCloneViewImpl();
        std::vector<EntityId> getScriptViewImpl();

        //----- Component management -----//
        void registerComponentImpl(ComponentRegistry* componentRegistry);// Used to register internal components and custom components
        void unregisterCustomComponentsImpl();// Unregister to free memory that was allocated for all custom components
        void createComponentPoolsFromRegistered();
        void createComponentPool(ComponentRegistry* componentRegistry);
        std::vector<ComponentRegistry*> getComponentRegistriesImpl() { return _componentRegistries; }
        BitmapAllocator* getComponentAllocator(ComponentRegistry* compReg);

        //----- Event handling -----//
        void onMeshEvent(Event& event);// Used the update the MeshComponent attribute options
        void onTextureEvent(Event& event);// Used the update the MaterialComponent attribute options
        void onScriptEvent(Event& event);// Used the update the ScriptComponent attribute options

        //----- Memory management -----//
        struct EntityBlock
        {
            void* components[maxRegisteredComponents];
        };
        EntityBlock* getEntityBlock(EntityId eid);

        StackAllocator* _allocator;// Used to allocate more memory to component pools
        StackAllocator::Marker _customComponentsMarker;// Marker to free custom components
        size_t _numAttaComponents;// Used to remove custom components form componentRegistries
        std::vector<ComponentRegistry*> _componentRegistries;// All registered components

        // Need to store this because old componentRegistry data is lost when component shared library is reloaded
        struct ComponentRegistryBackupInfo
        {
            size_t typeidHash;
            ComponentDescription description;
            bool poolCreated;
        };
        std::vector<ComponentRegistryBackupInfo> _componentRegistriesBackupInfo;

        // Entity views(TODO create views from template?)
        size_t _maxEntities;// Maximum number of entities
        std::set<EntityId> _entities;// View of entities
        std::set<EntityId> _noPrototypeView;// View of entities and clone (no prototype entity)
        std::set<EntityId> _cloneView;// View of only clones
        std::set<EntityId> _scriptView;// View of entities that are neither prototype or clone and have script component
        EntityId _selectedEntity = -1;// TODO Use views and selectedComponent to allow multi selection?

        //----- Factory Management -----//
        void onSimulationStateChange(Event& event);
        void createFactories();
        void destroyFactories();
        static EntityId createClones(size_t quantity);
        EntityId createClonesImpl(size_t quantity);
        std::vector<Factory>& getFactoriesImpl() { return _factories; }
        Factory* getPrototypeFactoryImpl(EntityId prototype);

        std::vector<Factory> _factories;
        friend Factory;
    };
}

#include <atta/component/componentManager.inl>
#endif// ATTA_COMPONENT_COMPONENT_MANAGER_H
