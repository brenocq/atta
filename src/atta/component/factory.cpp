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
#include <atta/script/interface.h>

namespace atta::component {
Factory::Factory(EntityId prototypeId) : _prototypeId(prototypeId) {
    Prototype* prototype = component::getComponent<Prototype>(_prototypeId);
    _maxClones = prototype->maxClones;
}

void Factory::createClones() {
    // Create entityId for each clone
    _firstCloneEid = component::Manager::getInstance().createClonesImpl(_maxClones);

    // FIXME not working recursive prototypes yet
    for (auto compReg : component::getComponentRegistries()) {
        // Check if prototype entity has this component
        Component* prototypeComponent = component::getComponentById(compReg->getId(), _prototypeId);
        if (prototypeComponent) {
            // XXX For now, just ignore the prototype component when clonnning
            if (compReg->getId() == COMPONENT_POOL_SID_BY_NAME(typeid(Prototype).name()))
                continue;

            // Get component allocator pool
            memory::BitmapAllocator* cpool = component::Manager::getInstance().getComponentAllocator(compReg);
            size_t componentSize = (size_t)compReg->getSizeof();

            // Copy default data from prototype entity component to clone components
            if (compReg->getId() != COMPONENT_POOL_SID_BY_NAME(typeid(Relationship).name())) {
                // Allocate memory for each clone
                uint8_t* mem = (uint8_t*)cpool->allocBytes(_maxClones * componentSize, componentSize);
                _componentMemories.push_back(std::make_pair(compReg->getId(), mem));
                _componentRegistries.push_back(compReg);

                for (unsigned i = 0; i < _maxClones; i++)
                    memcpy(mem + componentSize * i, prototypeComponent, componentSize);

                // Add allocated component to clone entities
                for (unsigned i = 0; i < _maxClones; i++)
                    component::addComponentPtr(_firstCloneEid + i, compReg->getIndex(), mem + componentSize * i);
            } else {
                // XXX Not sure how will work with the Relationship because it has std::vector
                // If the prototype entity has parent, set the parent as parent of clones as well
                Relationship* r = component::getComponent<Relationship>(_prototypeId);
                if (r->getParent() != -1) {
                    EntityId parentId = r->getParent();
                    r = component::getComponent<Relationship>(r->getParent());
                    for (EntityId entity = _firstCloneEid; entity < EntityId(_firstCloneEid + _maxClones); entity++)
                        r->addChild(parentId, entity);
                }
            }
        }
    }
}

void Factory::destroyClones() {
    for (EntityId i = _firstCloneEid; i < EntityId(_firstCloneEid + _maxClones); i++)
        component::deleteEntity(i);
    // TODO can be faster if deallocate each component and then deleteEntityOnly
}

void Factory::runScripts(float dt) {
    // TODO faster
    unsigned cloneId = 0;
    for (EntityId entity = _firstCloneEid; entity < EntityId(_firstCloneEid + _maxClones); entity++) {
        Script* scriptComponent = component::getComponent<Script>(entity);
        if (scriptComponent) {
            script::Script* script = script::getScript(scriptComponent->sid);
            if (script)
                script->update(Entity(entity, cloneId), dt);
        }
        cloneId++;
    }

    // Example using componentMemories to set Entity component vector
    // Script* scriptComponent = component::getComponent<script::Script>(_prototypeId);
    // if(scriptComponent)
    //{
    //    Script* script = script::getScript(scriptComponent->sid);
    //    if(script)
    //    {
    //        // Execute each clone script with the right component
    //        for(unsigned cloneId = 0; cloneId < _maxClones; cloneId++)
    //        {
    //            // Populate component memories
    //            for(unsigned i = 0; i < components.size(); i++)
    //                components[i] = reinterpret_cast<Component*>(_componentMemories[i].second +
    //                        cloneId * _componentRegistries[i]->getSizeof());

    //             Run script
    //            script->update(_prototypeId, cloneId, components, dt);
    //        }
    //    }
    //}
}

std::vector<ComponentId> Factory::getComponentsIds() const {
    std::vector<ComponentId> components;

    for (auto [componentId, memory] : _componentMemories)
        components.push_back(componentId);

    return components;
}

std::vector<uint8_t*> Factory::getMemories() const {
    std::vector<uint8_t*> memories;

    for (auto [componentId, memory] : _componentMemories)
        memories.push_back(memory);

    return memories;
}

std::vector<EntityId> Factory::getCloneIds() const {
    std::vector<EntityId> clones;
    clones.resize(_maxClones);
    int i = 0;
    for (EntityId entity = _firstCloneEid; entity < EntityId(_firstCloneEid + _maxClones); entity++, i++)
        clones[i] = entity;
    return clones;
}
} // namespace atta::component
