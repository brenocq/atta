//--------------------------------------------------
// Atta Component Module
// entity.h
// Date: 2021-09-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_ENTITY_H
#define ATTA_COMPONENT_ENTITY_H
#include <atta/component/base.h>

namespace atta::component {

class Entity {
  public:
    Entity();
    Entity(EntityId id);
    Entity(EntityId id, int cloneId);
    operator EntityId() const { return _id; }

    template <typename T>
    T* add() const {
        return component::addComponent<T>(_id);
    }

    template <typename T>
    T* get() const {
        return component::getComponent<T>(_id);
    }

    EntityId getId() const { return _id; }
    int getCloneId() const { return _cloneId; }

  private:
    EntityId _id;
    int _cloneId;
};

} // namespace atta::component

#endif // ATTA_COMPONENT_ENTITY_H