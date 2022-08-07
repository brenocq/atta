//--------------------------------------------------
// Atta Component Module
// entity.h
// Date: 2021-09-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_ENTITY_H
#define ATTA_COMPONENT_ENTITY_H
#include <atta/component/base.h>
#include <atta/component/manager.h>

namespace atta::component {
class Entity {
  public:
    Entity(EntityId id, int cloneId = 0);

    template <typename T>
    T* getComponent() {
        return component::Manager::getEntityComponent<T>(_id);
    }

    EntityId getId() const { return _id; }
    int getCloneId() const { return _cloneId; }

  private:
    EntityId _id;
    int _cloneId;
};
} // namespace atta::component

#endif // ATTA_COMPONENT_ENTITY_H
