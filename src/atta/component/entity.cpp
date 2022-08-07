//--------------------------------------------------
// Atta Component Module
// entity.cpp
// Date: 2021-09-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/entity.h>

namespace atta
{
    Entity::Entity(EntityId id, int cloneId):
        _id(id), _cloneId(cloneId)
    {

    }
}
