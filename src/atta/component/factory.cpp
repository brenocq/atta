//--------------------------------------------------
// Atta Component Module
// factory.cpp
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/componentRegistry.h>
#include <atta/component/components/prototype.h>
#include <atta/component/components/relationship.h>
#include <atta/component/components/script.h>
#include <atta/component/factory.h>
#include <atta/component/interface.h>
#include <atta/script/interface.h>

namespace atta::component {
Factory::Factory(Entity prototype) : _prototype(prototype), _numEntitiesCloned(0), _numEntitiesInitialized(0) {
    _maxClones = _prototype.get<Prototype>()->maxClones;
}

void Factory::createChildClones(Entity child, Entity parent) {
    // Create `_maxClones` clones of `child` and set all clones as child of `parent`
    // NOTE EntityId of custom entities/joints not working yet (only relationship being handled)

    Entity firstClone = EntityId(_firstClone) + _numEntitiesInitialized;

    // If the prototype entity has parent, set the parent as parent of clones as well
    if (parent != -1) {
        // clang-format off
        bool parentIsClone =
            parent.getId() >= _firstClone.getId() &&
            parent.getId() < EntityId(_firstClone.getId() + _numEntitiesCloned * _maxClones);
        // clang-format on

        Relationship* r = parent.get<Relationship>();
        for (unsigned i = 0; i < _maxClones; i++)
            r->addChild(parentIsClone ? Entity(parent + i) : parent, firstClone + i);
    }

    // For every registered component
    for (auto compReg : component::getComponentRegistries()) {
        // Check if child entity has this component
        Component* component = component::getComponentById(compReg->getId(), child);
        if (component) {
            // Ignore prototype component when clonning
            // TODO recursive prototypes not supported yet
            if (compReg->getId() == COMPONENT_POOL_SID_BY_NAME(typeid(Prototype).name()))
                continue;

            // Get component allocator pool
            memory::BitmapAllocator* cpool = component::Manager::getInstance().getComponentAllocator(compReg);
            size_t componentSize = (size_t)compReg->getSizeof();

            // Copy default data from prototype entity component to clone components
            // TODO components with EntityId variables not handled properly yet
            if (compReg->getId() != COMPONENT_POOL_SID_BY_NAME(typeid(Relationship).name())) {
                // Allocate memory for each clone
                uint8_t* mem = (uint8_t*)cpool->allocBytes(_maxClones * componentSize, componentSize);

                // Initialize component data
                for (unsigned i = 0; i < _maxClones; i++)
                    memcpy(mem + componentSize * i, component, componentSize);

                // Add allocated component to clone entities
                for (unsigned i = 0; i < _maxClones; i++)
                    component::addComponentPtr(firstClone.getId() + i, compReg->getIndex(), mem + componentSize * i);
            }
        }
    }
    _numEntitiesInitialized += _maxClones;

    // Recursive clone children
    Relationship* r = child.get<Relationship>();
    if (r)
        for (Entity childToClone : r->getChildren()) // For each child of prototype
            // Clone prototype child and set as clone child
            createChildClones(childToClone, firstClone);
}

void Factory::createClones() {
    // Count number of child entities
    _numEntitiesCloned = 1;
    Relationship* r = _prototype.get<Relationship>();
    if (r) {
        std::stack<std::vector<Entity>> s;
        s.push(r->getChildren());
        while (!s.empty()) {
            std::vector<Entity> children = s.top();
            s.pop();
            _numEntitiesCloned += children.size();
            for (Entity child : children) {
                r = child.get<Relationship>();
                if (r)
                    s.push(r->getChildren());
            }
        }
    }

    // Allocate one entity of each clone
    _firstClone = component::Manager::getInstance().createClonesImpl(_maxClones * _numEntitiesCloned);

    // Create and initialize clone components (recursivelly)
    _numEntitiesInitialized = 0;
    r = _prototype.get<Relationship>();
    createChildClones(_prototype, r ? r->getParent() : Entity(-1));
}

void Factory::destroyClones() {
    // Destroy from back to front because need to destroy children first
    for (int i = _maxClones * _numEntitiesCloned - 1; i >= 0; i--)
        component::deleteEntity(_firstClone.getId() + i);
    _numEntitiesCloned = 0;
    _numEntitiesInitialized = 0;
}

Entity Factory::getPrototype() const { return _prototype; }
Entity Factory::getFirstClone() const { return _firstClone; }
uint64_t Factory::getNumEntitiesCloned() const { return _numEntitiesCloned; }
uint64_t Factory::getMaxClones() const { return _maxClones; }

std::vector<Entity> Factory::getClones(bool includeChildren) const {
    std::vector<Entity> clones;
    unsigned size = includeChildren ? _maxClones * _numEntitiesCloned : _maxClones;
    clones.resize(size);
    for (unsigned i = 0; i < clones.size(); i++)
        clones[i] = Entity(_firstClone.getId() + i);
    return clones;
}

bool Factory::isClone(Entity entity) {
    return entity.getId() >= _firstClone.getId() && entity.getId() < _firstClone.getId() + _maxClones * _numEntitiesCloned;
}

bool Factory::isRootClone(Entity entity) { return entity.getId() >= _firstClone.getId() && entity.getId() < _firstClone.getId() + _maxClones; }

} // namespace atta::component
