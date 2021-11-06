//--------------------------------------------------
// Atta Component System
// prototypeComponent.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>

namespace atta
{
    struct PrototypeComponent final : public Component
    {
        uint64_t maxClones;
    };
    ATTA_REGISTER_COMPONENT(PrototypeComponent)

    template<>
    inline ComponentRegistry::Description TypedComponentRegistry<PrototypeComponent>::description = 
    {
        "Prototype",
        {
            { ComponentRegistry::AttributeType::UINT64, offsetof(PrototypeComponent, maxClones), "maxClones", uint64_t(1), uint64_t(1000000) }
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
