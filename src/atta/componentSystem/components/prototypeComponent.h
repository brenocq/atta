//--------------------------------------------------
// Atta Component System
// prototypeComponent.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
#include <atta/componentSystem/components/component.h>

namespace atta
{
    struct PrototypeComponent final : public TypedComponent<PrototypeComponent>
    {
        uint64_t maxClones;
    };

    template<>
    inline const Component::TypeDescription TypedComponent<PrototypeComponent>::typeDescription = 
    {
        "Prototype",
        {
            { Component::AttributeType::UINT64, offsetof(PrototypeComponent, maxClones), "maxClones", uint64_t(1), uint64_t(1000000) }
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
