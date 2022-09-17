//--------------------------------------------------
// Atta Component Module
// factory.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_FACTORY_H
#define ATTA_COMPONENT_FACTORY_H
#include <atta/component/base.h>

namespace atta::component {
class Entity;
class ComponentRegistry;
class Factory {
  public:
    // The entity must be a prototype entity (has prototype component)
    Factory(EntityId prototypeId);

    void createClones();
    void destroyClones();

    template <typename T>
    T* getComponent(uint64_t cloneId = 0);
    void runScripts(float dt);

    EntityId getPrototypeId() const;
    EntityId getFirstCloneId() const;
    std::vector<Entity> getClones() const;
    std::vector<EntityId> getCloneIds() const;
    Entity getFirstClone() const;
    Entity getLastClone() const;
    uint64_t getMaxClones() const;
    uint64_t getNumClones() const;
    std::vector<std::pair<ComponentId, uint8_t*>>& getComponentMemories();
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
} // namespace atta::component

#include <atta/component/factory.inl>
#endif // ATTA_COMPONENT_ENTITY_H