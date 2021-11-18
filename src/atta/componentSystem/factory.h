//--------------------------------------------------
// Atta Component System
// factory.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_FACTORY_H
#define ATTA_COMPONENT_SYSTEM_FACTORY_H
#include <atta/componentSystem/base.h>

namespace atta
{
    class ComponentRegistry;
    class Factory
    {
    public:
        // The entity must be a prototype entity (has prototype component)
        Factory(EntityId prototypeId);

        void createClones();
        void destroyClones();

        template <typename T>
        T* getComponent(uint64_t cloneId = 0);
        void runScripts(float dt);

        EntityId getPrototypeId() const { return _prototypeId; }
        uint64_t getMaxClones() const { return _maxClones; }
        std::vector<std::pair<ComponentId, uint8_t*>>& getComponentMemories() { return _componentMemories; }
        std::vector<ComponentId> getComponentsIds() const;
        std::vector<uint8_t*> getMemories() const;

    private:

        EntityId _prototypeId;
        uint64_t _maxClones;
        EntityId _firstCloneEid;

        // Map the hash of each type to the memory start
        // Using vector assuming just a few of components
        std::vector<std::pair<ComponentId, uint8_t*>> _componentMemories;
        std::vector<ComponentRegistry*> _componentRegistries;
    };
}

#include <atta/componentSystem/factory.inl>
#endif// ATTA_COMPONENT_SYSTEM_ENTITY_H
