//--------------------------------------------------
// Atta Component System
// entity.h
// Date: 2021-09-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_ENTITY_H
#define ATTA_COMPONENT_SYSTEM_ENTITY_H
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/base.h>

namespace atta
{
    class Entity
    {
    public:
        Entity(EntityId id);

        template <typename T>
        T* getComponent() { return ComponentManager::getEntityComponent<T>(_id); }

    private:
        EntityId _id;
    };
}

#endif// ATTA_COMPONENT_SYSTEM_ENTITY_H
