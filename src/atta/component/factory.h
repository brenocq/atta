//--------------------------------------------------
// Atta Component Module
// factory.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_FACTORY_H
#define ATTA_COMPONENT_FACTORY_H

namespace atta::component {
class Entity;
class Factory {
  public:
    // The entity must be a prototype entity (has prototype component)
    Factory(Entity prototype);

    void createClones();
    void destroyClones();

    Entity getPrototype() const;
    Entity getFirstClone() const;
    uint64_t getMaxClones() const;
    uint64_t getNumEntitiesCloned() const;
    std::vector<Entity> getClones(bool includeChildren = false) const;
    bool isClone(Entity entity);///< Check if entity is clone from this factory
    bool isRootClone(Entity entity);///< Check if entity is root clone from this factory

  private:
    void createChildClones(Entity child, Entity parent = -1);

    Entity _prototype;   ///< Prototype entity that will be cloned
    uint64_t _maxClones; ///< Number of clones of each entity to create
    Entity _firstClone;  ///< First clone created

    uint64_t _numEntitiesCloned;      ///< Number of entities cloned (prototype entity and its children)
    uint64_t _numEntitiesInitialized; ///< Auxiliar variable to keep track of component creation
};
} // namespace atta::component

#endif // ATTA_COMPONENT_ENTITY_H
