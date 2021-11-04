//--------------------------------------------------
// Atta Component System
// nameComponent.cpp
// Date: 2021-11-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/nameComponent.h>

namespace atta
{
    template<>
    const Component::TypeDescription TypedComponent<NameComponent>::typeDescription = 
    {
        Component::Type("Name"),
        {
            { Component::AttributeType::CHAR, attributeOffset(&NameComponent::name), "name" }
        }
    };
}
