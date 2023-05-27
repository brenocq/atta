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

#include <atta/component/factory.h>
#include <atta/memory/allocators/bitmapAllocator.h>
#include <atta/memory/allocators/poolAllocatorT.h>
#include <atta/memory/allocators/stackAllocator.h>
#include <atta/memory/interface.h>

namespace atta::component {

constexpr unsigned maxRegisteredComponents = 32;
constexpr unsigned maxEntities = 1024;

class Registry;
class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend Entity createEntity(EntityId entity);
    friend void deleteEntity(Entity entity);
    friend Entity copyEntity(Entity entity);
    template <typename T>
    friend T* addComponent(Entity entity);
    friend Component* addComponentById(ComponentId id, Entity entity);
    friend Component* addComponentPtr(Entity entity, unsigned index, Component* component);
    template <typename T>
    friend T* getComponent(Entity entity);
    friend Component* getComponentById(ComponentId id, Entity entity);
    friend std::vector<Component*> getComponents(Entity entity);
    friend void removeComponentById(ComponentId id, Entity entity);
    friend std::vector<Registry*> getComponentRegistries();
    friend std::vector<Factory>& getFactories();
    friend Factory* getFactory(Entity prototype);
    friend std::vector<EntityId> getEntitiesView();
    friend std::vector<EntityId> getNoPrototypeView();
    friend std::vector<EntityId> getCloneView();
    friend std::vector<EntityId> getNoCloneView();
    friend std::vector<EntityId> getScriptView();
    friend Entity getSelectedEntity();
    friend void setSelectedEntity(Entity entity);
    friend void createDefault();
    friend void clear();
    friend void unregisterCustomComponents();

  private:
    //----- Startup/ShutDown -----//
    void startUpImpl();
    void shutDownImpl();
    void createDefaultImpl();
    void clearImpl();
    void createEntityPool();

    //----- Entity -----//
    Entity createEntityImpl(EntityId entity = -1, size_t quantity = 1);
    void deleteEntityImpl(Entity entity);
    Entity copyEntityImpl(Entity entity);

    //----- Component -----//
    template <typename T>
    T* addComponentImpl(Entity entity);
    Component* addComponentByIdImpl(ComponentId id, Entity entity);
    Component* addComponentPtrImpl(Entity entity, unsigned index, Component* component);
    void notifyComponentAdded(Entity entity, ComponentId id, Component* ptr);
    void removeComponentByIdImpl(ComponentId id, Entity entity);
    void notifyComponentRemoved(Entity entity, ComponentId id);

    template <typename T>
    T* getComponentImpl(Entity entity);
    Component* getComponentByIdImpl(ComponentId id, Entity entity);
    Component* getComponentByIndex(unsigned index, Entity entity);
    std::vector<Component*> getComponentsImpl(Entity entity);

    //----- Views -----//
    // TODO Use template to allow multiple views to be registered
    std::vector<EntityId> getEntitiesViewImpl();
    std::vector<EntityId> getNoPrototypeViewImpl();
    std::vector<EntityId> getCloneViewImpl();
    std::vector<EntityId> getNoCloneViewImpl();
    std::vector<EntityId> getScriptViewImpl();
    std::set<EntityId> _entities;        // All entities
    std::set<EntityId> _noPrototypeView; // View of entities and clone (no prototype entity)
    std::set<EntityId> _cloneView;       // View of only clones
    std::set<EntityId> _scriptView;      // View of entities that are neither prototype or clone and have script component

    //----- Factory Management -----//
    void onSimulationStateChange(event::Event& event);
    void createFactories();
    void destroyFactories();
    EntityId createClonesImpl(size_t quantity);
    std::vector<Factory>& getFactoriesImpl() { return _factories; }
    Factory* getFactoryImpl(Entity prototype);

    std::vector<Factory> _factories;
    friend Factory;

    //----- Component management -----//
    void unregisterCustomComponentsImpl();                            // Unregister to free memory that was allocated for all custom components
    void createComponentPoolsFromRegistered();
    void createComponentPool(Registry* componentRegistry);
    memory::BitmapAllocator* getComponentAllocator(Registry* compReg);
    Registry* getRegistry(ComponentId id);
    std::vector<Registry*> getComponentRegistriesImpl();

    //----- Event handling -----//
    void onMeshEvent(event::Event& event);   // Used the update the Mesh attribute options
    void onImageEvent(event::Event& event);  // Used the update the Material attribute options
    void onScriptEvent(event::Event& event); // Used the update the Script attribute options

    //----- Memory management -----//
    struct EntityBlock {
        Component* components[maxRegisteredComponents];
    };
    EntityBlock* getEntityBlock(EntityId eid);

    // Stack allocators to manage component pools
    memory::StackAllocator* _cpuAllocator;
    memory::StackAllocator* _gpuAllocator;
    uint8_t* _gpuGlobalMemory; // GPU global memory (used by _gpuAllocator)

    void copyGpuToCpu();
    void copyCpuToGpu();

    // Need to store this because old componentRegistry data is lost when component shared library is reloaded
    //struct RegistryBackupInfo {
    //    size_t typeidHash;
    //    ComponentDescription description;
    //    bool poolCreated;
    //};
    //std::vector<RegistryBackupInfo> _componentRegistriesBackupInfo;

    //// Bookkeeping component/entity pools
    //memory::StackAllocator::Marker _customComponentsMarker; // Marker to free custom components
    //std::vector<Registry*> _componentRegistries;   // All registered components
    size_t _numAttaComponents;                              // Used to remove custom components from componentRegistries
    size_t _maxEntities;                                    // Maximum number of entities

    //----- Custom -----//
    EntityId _selectedEntity = -1;
};

} // namespace atta::component

#include <atta/component/manager.inl>

#endif // ATTA_COMPONENT_MANAGER_H
