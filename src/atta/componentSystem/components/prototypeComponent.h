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
    template<> ComponentRegistry::Description& TypedComponentRegistry<PrototypeComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
