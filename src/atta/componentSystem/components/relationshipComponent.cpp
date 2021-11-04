//--------------------------------------------------
// Atta Component System
// relationshipComponent.h
// Date: 2021-11-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/relationshipComponent.h>

namespace atta
{
    template<>
    const Component::TypeDescription TypedComponent<RelationshipComponent>::typeDescription = 
    {
        Component::Type("Relationship"),
        {
            { Component::AttributeType::UINT32, attributeOffset(&RelationshipComponent::parent), "parent" },
            { Component::AttributeType::CUSTOM, attributeOffset(&RelationshipComponent::children), "children" }
        }
    };
}
