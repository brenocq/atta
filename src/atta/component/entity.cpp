//--------------------------------------------------
// Atta Component Module
// entity.cpp
// Date: 2021-09-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/interface.h>

#include <atta/component/entity.h>

namespace atta::component {

Entity::Entity() : Entity(-1, -1) {}
Entity::Entity(EntityId id) : Entity(id, 0) {}
Entity::Entity(EntityId id, int cloneId) : _id(id), _cloneId(cloneId) {}

} // namespace atta::component
