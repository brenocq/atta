//--------------------------------------------------
// Atta Component Module
// manager.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_MANAGER_H
#define ATTA_COMPONENT_MANAGER_H

// XXX for now manage.h should not be included, include interface.h instead

#include <atta/event/interface.h>

#include <atta/memory/allocators/bitmapAllocator.h>
#include <atta/memory/allocators/poolAllocatorT.h>
#include <atta/memory/allocators/stackAllocator.h>
#include <atta/memory/interface.h>

namespace atta::component {

constexpr unsigned maxRegisteredComponents = 32;
constexpr unsigned maxEntities = 1024;

class ComponentRegistry;
class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend EntityId createEntity(EntityId entity);
    friend void deleteEntity(EntityId entity);
    friend void deleteEntityOnly(EntityId entity);
    friend EntityId copyEntity(EntityId entity);
    template <typename T>
    friend T* addEntityComponent(EntityId entity);
    friend Component* addEntityComponentById(ComponentId id, EntityId entity);
    friend Component* addEntityComponentPtr(EntityId entity, unsigned index, uint8_t* component);
    template <typename T>
    friend T* getEntityComponent(EntityId entity);
    friend Component* getEntityComponentById(ComponentId id, EntityId entity);
    friend std::vector<Component*> getEntityComponents(EntityId entity);
    friend void removeEntityComponentById(ComponentId id, EntityId entity);
    friend std::vector<ComponentRegistry*> getComponentRegistries();
    friend std::vector<Factory>& getFactories();
    friend Factory* getPrototypeFactory(EntityId prototype);
    friend std::vector<EntityId> getEntitiesView();
    friend std::vector<EntityId> getNoPrototypeView();
    friend std::vector<EntityId> getCloneView();
    friend std::vector<EntityId> getNoCloneView();
    friend std::vector<EntityId> getScriptView();
    friend EntityId getSelectedEntity();
    friend void setSelectedEntity(EntityId eid);
    friend void createDefault();
    friend void clear();
    friend void registerComponent(ComponentRegistry* componentRegistry);
    friend void unregisterCustomComponents();

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
    void registerComponentImpl(ComponentRegistry* componentRegistry); // Used to register internal components and custom components
    void unregisterCustomComponentsImpl();                            // Unregister to free memory that was allocated for all custom components
    void createComponentPoolsFromRegistered();
    void createComponentPool(ComponentRegistry* componentRegistry);
    std::vector<ComponentRegistry*> getComponentRegistriesImpl() { return _componentRegistries; }
    memory::BitmapAllocator* getComponentAllocator(ComponentRegistry* compReg);

    //----- Event handling -----//
    void onMeshEvent(event::Event& event);   // Used the update the Mesh attribute options
    void onImageEvent(event::Event& event);  // Used the update the Material attribute options
    void onScriptEvent(event::Event& event); // Used the update the Script attribute options

    //----- Memory management -----//
    struct EntityBlock {
        void* components[maxRegisteredComponents];
    };
    EntityBlock* getEntityBlock(EntityId eid);

    memory::StackAllocator* _allocator;                     // Used to allocate more memory to component pools
    memory::StackAllocator::Marker _customComponentsMarker; // Marker to free custom components
    size_t _numAttaComponents;                              // Used to remove custom components form componentRegistries
    std::vector<ComponentRegistry*> _componentRegistries;   // All registered components

    // Need to store this because old componentRegistry data is lost when component shared library is reloaded
    struct ComponentRegistryBackupInfo {
        size_t typeidHash;
        ComponentDescription description;
        bool poolCreated;
    };
    std::vector<ComponentRegistryBackupInfo> _componentRegistriesBackupInfo;

    // Entity views(TODO create views from template?)
    size_t _maxEntities;                 // Maximum number of entities
    std::set<EntityId> _entities;        // View of entities
    std::set<EntityId> _noPrototypeView; // View of entities and clone (no prototype entity)
    std::set<EntityId> _cloneView;       // View of only clones
    std::set<EntityId> _scriptView;      // View of entities that are neither prototype or clone and have script component
    EntityId _selectedEntity = -1;       // TODO Use views and selectedComponent to allow multi selection?

    //----- Factory Management -----//
    void onSimulationStateChange(event::Event& event);
    void createFactories();
    void destroyFactories();
    EntityId createClonesImpl(size_t quantity);
    std::vector<Factory>& getFactoriesImpl() { return _factories; }
    Factory* getPrototypeFactoryImpl(EntityId prototype);

    std::vector<Factory> _factories;
    friend Factory;
};

} // namespace atta::component

#include <atta/component/manager.inl>

#endif // ATTA_COMPONENT_MANAGER_H
