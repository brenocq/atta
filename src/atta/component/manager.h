//--------------------------------------------------
// Atta Component Module
// manager.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_MANAGER_H
#define ATTA_COMPONENT_MANAGER_H

// XXX for now manage.h should not be included, include interface.h instead

#include <atta/component/entity.h>
#include <atta/component/factory.h>
#include <atta/event/interface.h>
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
    friend void destroyEntity(Entity entity);
    friend Entity copyEntity(Entity entity);
    friend std::vector<Factory>& getFactories();
    friend Factory* getFactory(Entity prototype);
    friend std::vector<Entity> getEntitiesView();
    friend std::vector<Entity> getNoPrototypeView();
    friend std::vector<Entity> getScriptView();
    friend void createDefault();
    friend void clear();

  private:
    //----- Startup/ShutDown -----//
    void startUpImpl();
    void shutDownImpl();
    void createDefaultImpl();
    void clearImpl();

    //----- Entity -----//
    Entity createEntityImpl(EntityId entity = -1);
    void destroyEntityImpl(Entity entity);
    Entity copyEntityImpl(Entity entity);

    //----- Views -----//
    // TODO Use template to allow custom views to be registered
    std::vector<Entity> getEntitiesViewImpl();
    std::vector<Entity> getNoPrototypeViewImpl();
    std::vector<Entity> getScriptViewImpl();

    //----- Factory -----//
    void onSimulationStateChange(event::Event& event);
    void createFactories();
    void destroyFactories();
    std::vector<Factory>& getFactoriesImpl();
    Factory* getFactoryImpl(Entity prototype);

    std::vector<Factory> _factories;
    friend Factory;

    //----- Memory -----//
    memory::BitmapAllocator* _allocator;

    //----- Event handling -----//
    void onMeshEvent(event::Event& event);   // Used the update the Mesh attribute options
    void onImageEvent(event::Event& event);  // Used the update the Material attribute options
    void onScriptEvent(event::Event& event); // Used the update the Script attribute options
};

} // namespace atta::component

#endif // ATTA_COMPONENT_MANAGER_H
