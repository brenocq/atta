//--------------------------------------------------
// Atta Component System
// prototypeComponent.h
// Date: 2021-11-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/prototypeComponent.h>

namespace atta
{
    template<>
    const Component::TypeDescription TypedComponent<PrototypeComponent>::typeDescription = 
    {
        Component::Type("Prototype"),
        {
            { Component::AttributeType::UINT64, attributeOffset(&PrototypeComponent::maxClones), "maxClones", uint64_t(1), uint64_t(1000000) }
        }
    };
}
