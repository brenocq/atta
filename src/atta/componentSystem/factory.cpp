//--------------------------------------------------
// Atta Component System
// factory.cpp
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/factory.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/componentSystem/components/prototypeComponent.h>
#include <atta/componentSystem/components/scriptComponent.h>
#include <atta/scriptSystem/scriptManager.h>

namespace atta
{
    Factory::Factory(EntityId prototypeId):
        _prototypeId(prototypeId)
    {
        PrototypeComponent* prototype = ComponentManager::getEntityComponent<PrototypeComponent>(_prototypeId);
        _maxClones = prototype->maxClones;
    }

    void Factory::createClones()
    {
        std::vector<std::pair<ComponentId, uint8_t*>> componentMemories;
        _cloneEntityIds.resize(_maxClones);

        // Create entityId for each clone (TODO ensure continuous memory)
        for(uint32_t i = 0; i < _maxClones; i++)
            _cloneEntityIds[i] = ComponentManager::createClone();

        for(auto compReg : ComponentManager::getComponentRegistries())
        {
            // Check if prototype entity has this component
            Component* prototypeComponent = ComponentManager::getEntityComponentById(compReg->getId(), _prototypeId);
            if(prototypeComponent)
            {
                // For now, just ignore the prototype component when clonnning
                if(compReg->getId() == COMPONENT_POOL_SID_BY_NAME(typeid(PrototypeComponent).name()))
                    continue;

                // Get component allocator pool
                PoolAllocator* cpool = MemoryManager::getAllocator<PoolAllocator>(compReg->getId());
                size_t componentSize = (size_t)compReg->getSizeof();

                // Allocate memory for each clone (TODO ensure continuous memory)
                uint8_t* mem = (uint8_t*)cpool->alloc();
                componentMemories.push_back(std::make_pair(compReg->getId(), mem));
                for(unsigned i = 1; i < _maxClones; i++)
                    cpool->alloc();

                // Copy default data from prototype entity component to clone components
                for(unsigned i = 0; i < _maxClones; i++)
                    memcpy(mem+componentSize*i, prototypeComponent, componentSize);

                // Add allocated component to clone entities
                for(unsigned i = 0; i < _maxClones; i++)
                    ComponentManager::addEntityComponentPtr(_cloneEntityIds[i], mem+componentSize*i);
            }
        }

        for(auto [componentId, memory] : _componentMemories)
            for(auto compReg : ComponentManager::getComponentRegistries())
                if(compReg->getId() == componentId)
                    _componentRegistries.push_back(compReg);
    }

    void Factory::destroyClones()
    {
        // Release memory for each factory
        // Because we are using a pool free list allocator, need to delete from last to first element
        for(auto [componentId, memory] : getComponentMemories())
        {
            ComponentRegistry* compReg;
            // Find component registry
            for(auto cr : _componentRegistries)
                if(cr->getId() == componentId)
                {
                    compReg = cr;
                    break;
                }

            //LOG_WARN("ComponentManager", "Deleting component $0, mem:$1", _componentNames[componentHash], (void*)memory);
            // Delete from last to first
            size_t componentSize = compReg->getSizeof();
            size_t blockSize = std::max(sizeof(void*), componentSize);
            PoolAllocator* cpool = MemoryManager::getAllocator<PoolAllocator>(componentId);

            for(uint8_t* it = memory+blockSize*_maxClones; it >= memory; it-=blockSize)
                cpool->free(it);
        }

        // Delete entity component
        for(EntityId eid : _cloneEntityIds)
            ComponentManager::destroyEntityOnly(eid);
    }

    void Factory::runScripts(float dt)
    {
        //std::vector<Component*> components;
        //components.resize(_componentMemories.size());

        // TODO faster
        unsigned cloneId = 0;
        for(EntityId entity : _cloneEntityIds)
        {
            ScriptComponent* scriptComponent = ComponentManager::getEntityComponent<ScriptComponent>(entity);
            if(scriptComponent)
            {
                Script* script = ScriptManager::getScript(scriptComponent->sid);
                if(script)
                    script->update(Entity(entity, cloneId), dt);
            }
            cloneId++;
        }

        // Example using componentMemories to set Entity component vector
        //ScriptComponent* scriptComponent = ComponentManager::getEntityComponent<ScriptComponent>(_prototypeId);
        //if(scriptComponent)
        //{
        //    Script* script = ScriptManager::getScript(scriptComponent->sid);
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

    std::vector<ComponentId> Factory::getComponentsIds() const
    {
        std::vector<ComponentId> components;

        for(auto [componentId, memory] : _componentMemories)
            components.push_back(componentId);

        return components;
    }

    std::vector<uint8_t*> Factory::getMemories() const
    {
        std::vector<uint8_t*> memories;

        for(auto [componentId, memory] : _componentMemories)
            memories.push_back(memory);

        return memories;
    }
}
