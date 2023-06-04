//--------------------------------------------------
// Atta Component Module
// manager.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/components.h>
#include <atta/component/dataManager/cpuDataManager.h>
#include <atta/component/dataManager/gpuDataManager.h>
#include <atta/component/factory.h>
#include <atta/component/interface.h>
#include <atta/component/manager.h>
#include <atta/event/events/createEntity.h>
#include <atta/event/events/destroyEntity.h>
#include <atta/event/events/imageLoad.h>
#include <atta/event/events/meshLoad.h>
#include <atta/event/events/scriptTarget.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/material.h>
#include <atta/utils/cuda.h>
#include <cstring>

namespace atta::component {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    //----- Memory -----//
    // Get main memory
    memory::Allocator* mainAllocator = memory::getAllocator(SSID("MainAllocator"));
    size_t size = 2UL * 1024UL * 1024UL * 1024UL; // 1GB
    // Alloc memory inside main memory
    uint8_t* componentMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
    ASSERT(componentMemory != nullptr, "Could not allocate component system memory");

    // Create new allocator from component memory
    _allocator = new memory::BitmapAllocator(componentMemory, size);
    memory::registerAllocator("ComponentAllocator", static_cast<memory::Allocator*>(_allocator));

    //----- DataManager -----//
    CpuDataManager::init();
    GpuDataManager::init();

    // Create pools
    for (Registry* r : Registry::get()) {
        // TODO hardcoded decision between 1 and many instances
        uint32_t numInstances = DataManager::maxEntities; // r->getDescription().maxInstances;
        if (r->getSizeof() >= 10 * 1024)
            numInstances = 1;
        cpuDataManager->allocPool(r->getId(), numInstances);
        GpuDataManager::allocPool(r->getId(), numInstances);
    }

    //----- Events -----//
    event::subscribe<event::MeshLoad>(BIND_EVENT_FUNC(Manager::onMeshEvent));
    event::subscribe<event::ImageLoad>(BIND_EVENT_FUNC(Manager::onImageEvent));
    event::subscribe<event::ScriptTarget>(BIND_EVENT_FUNC(Manager::onScriptEvent));

    // Default entity
    createDefaultImpl();
}

void Manager::shutDownImpl() {
    GpuDataManager::deinit();
    CpuDataManager::deinit();
}

void Manager::createDefaultImpl() {
    // Cube entity
    Entity cube = createEntity();
    cube.add<Transform>();
    cube.add<Name>()->set("Cube");
    cube.add<Mesh>()->set("meshes/cube.obj");

    resource::Material* rmt = resource::create<resource::Material>("Material", resource::Material::CreateInfo{});
    rmt->color = {0.5, 0.5, 0.5};
    cube.add<Material>()->set(rmt);

    // Light entity
    Entity light = createEntity();
    Transform* t = light.add<Transform>();
    t->position = {0.6, 0.6, 0.6};
    t->scale = {0.05, 0.05, 0.05};
    light.add<Name>()->set("Point light");
    light.add<Mesh>()->set("meshes/sphere.obj");
    light.add<PointLight>();
}

void Manager::clearImpl() {
    std::vector<Entity> entities = getEntitiesViewImpl();
    for (Entity entity : entities)
        cpuDataManager->destroyEntity(entity);
}

Entity Manager::createEntityImpl(EntityId entity) {
    Entity created = cpuDataManager->createEntity(entity);

    event::CreateEntity event;
    event.entityId = created.getId();
    event::publish(event);

    return created;
}

void Manager::destroyEntityImpl(Entity entity) {
    cpuDataManager->destroyEntity(entity);

    event::DestroyEntity event;
    event.entityId = entity.getId();
    event::publish(event);
}

Entity Manager::copyEntityImpl(Entity entity) {
    Entity copy = cpuDataManager->createEntity();
    for (ComponentId cid : cpuDataManager->getComponents(entity)) {
        Component* cCopy = cpuDataManager->addComponent(copy, cid);
        Component* cOrig = cpuDataManager->getComponent(entity, cid);
        uint32_t componentSize = Registry::get(cid)->getSizeof();
        memcpy(cCopy, cOrig, componentSize);
    }
    return copy;
}

//----------------------------------------//
//----------------- Views ----------------//
//----------------------------------------//
std::vector<Entity> Manager::getEntitiesViewImpl() {
    std::vector<EntityId> eids = cpuDataManager->getEntities();
    std::vector<Entity> entities;
    entities.reserve(eids.size());
    for (EntityId eid : eids)
        entities.push_back(Entity{eid});
    return entities;
}

std::vector<Entity> Manager::getNoPrototypeViewImpl() {
    std::vector<Entity> entities;
    std::vector<Entity> allEntities = getEntitiesViewImpl();
    for (Entity entity : allEntities)
        if (entity.get<Prototype>() == nullptr)
            entities.push_back(entity);
    return entities;
}

std::vector<Entity> Manager::getScriptViewImpl() {
    std::vector<Entity> entities;
    std::vector<Entity> allEntities = getEntitiesViewImpl();
    for (Entity entity : allEntities)
        if (entity.get<Script>())
            entities.push_back(entity);
    return entities;
}

//----------------------------------------//
//---------------- Factory ---------------//
//----------------------------------------//
void Manager::createFactories() {
    // Create factory for each entity that has prototype
    _factories.clear();
    for (Entity entity : getEntitiesView()) {
        Prototype* prototype = entity.get<Prototype>();
        if (prototype && prototype->maxClones > 0) {
            _factories.emplace_back(entity);
            _factories.back().createClones();
        }
    }
}

void Manager::destroyFactories() {
    for (int i = _factories.size() - 1; i >= 0; i--)
        _factories[i].destroyClones();
    _factories.clear();
}

std::vector<Factory>& Manager::getFactoriesImpl() { return _factories; }

Factory* Manager::getFactoryImpl(Entity prototype) {
    for (Factory& factory : _factories)
        if (factory.getPrototype() == prototype)
            return &factory;
    LOG_WARN("component::Manager", "Trying to get factory from entity [w]$0[] that is not a prototype", prototype);
    return nullptr;
}

//----------------------------------------//
//--------------- Events -----------------//
//----------------------------------------//
void Manager::onMeshEvent(event::Event& event) {
    switch (event.getType()) {
        case event::MeshLoad::type: {
            event::MeshLoad& e = reinterpret_cast<event::MeshLoad&>(event);
            bool found = false;
            for (auto op : TypedRegistry<Mesh>::description->attributeDescriptions[0].options)
                if (std::any_cast<StringId>(op) == e.sid) {
                    found = true;
                    break;
                }

            if (!found)
                TypedRegistry<Mesh>::description->attributeDescriptions[0].options.push_back(std::any(e.sid));

            break;
        }
        default:
            break;
    }
}

void Manager::onImageEvent(event::Event& event) {
    switch (event.getType()) {
        case event::ImageLoad::type: {
            event::ImageLoad& e = reinterpret_cast<event::ImageLoad&>(event);

            // Update environment light options
            {
                bool found = false;
                for (auto op : TypedRegistry<EnvironmentLight>::description->attributeDescriptions[0].options)
                    if (std::any_cast<StringId>(op) == e.sid) {
                        found = true;
                        break;
                    }

                if (!found) {
                    TypedRegistry<EnvironmentLight>::description->attributeDescriptions[0].options.push_back(std::any(e.sid));
                }
            }

            break;
        }
        default:
            break;
    }
}

void Manager::onScriptEvent(event::Event& event) {
    event::ScriptTarget& e = reinterpret_cast<event::ScriptTarget&>(event);

    TypedRegistry<Script>::description->attributeDescriptions[0].options.clear();
    for (StringId script : e.scriptSids)
        TypedRegistry<Script>::description->attributeDescriptions[0].options.push_back(std::any(script));
}

} // namespace atta::component
