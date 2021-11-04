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

    template<> const Component::TypeDescription TypedComponent<PrototypeComponent>::typeDescription;
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
