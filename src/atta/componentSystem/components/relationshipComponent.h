//--------------------------------------------------
// Atta Component System
// relationshipComponent.h
// Date: 2021-09-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_RELATIONSHIP_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_RELATIONSHIP_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/base.h>

namespace atta
{
    struct RelationshipComponent final : public TypedComponent<RelationshipComponent>
    {
        EntityId parent;
        std::vector<EntityId> children;
    };

    template<>
    inline const Component::TypeDescription TypedComponent<RelationshipComponent>::typeDescription = 
    {
        "Relationship",
        {
            { Component::AttributeType::UINT32, offsetof(RelationshipComponent, parent), "parent" },
            { Component::AttributeType::CUSTOM, offsetof(RelationshipComponent, children), "children" }
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_RELATIONSHIP_COMPONENT_H

