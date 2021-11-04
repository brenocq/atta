//--------------------------------------------------
// Atta Component System
// nameComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_NAME_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_NAME_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/core/math/math.h>

namespace atta
{
    struct NameComponent final : public TypedComponent<NameComponent>
    {
        char name[32];
    };

    template<>
    inline const Component::TypeDescription TypedComponent<NameComponent>::typeDescription = 
    {
        "Name",
        {
            { Component::AttributeType::CHAR, offsetof(NameComponent, name), "name" }
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_NAME_COMPONENT_H
