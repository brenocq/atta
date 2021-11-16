//--------------------------------------------------
// Atta Component System
// relationshipComponent.h
// Date: 2021-09-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_RELATIONSHIP_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_RELATIONSHIP_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/componentSystem/base.h>
#include <atta/fileSystem/serializer/serializer.h>

namespace atta
{
    struct RelationshipComponent final : public Component
    {
        // To keep the hierarchy consistent, this component can only be changed using these functions
        // Parent operations
        static void setParent(EntityId parent, EntityId child);
        static void removeParent(EntityId parent, EntityId child);
        // Child operations
        static void addChild(EntityId parent, EntityId child);
        static void removeChild(EntityId parent, EntityId child);

        // Get data
        EntityId getParent() { return _parent; }
        std::vector<EntityId> getChildren() { return _children; }

        // Data
        EntityId _parent = -1;
        std::vector<EntityId> _children;
    };
    ATTA_REGISTER_COMPONENT(RelationshipComponent)

    template<>
    inline ComponentRegistry::Description TypedComponentRegistry<RelationshipComponent>::description = 
    {
        "Relationship",
        {
            { ComponentRegistry::AttributeType::UINT32, offsetof(RelationshipComponent, _parent), "parent" },
            { ComponentRegistry::AttributeType::CUSTOM, offsetof(RelationshipComponent, _children), "children" }
        },
        // Max instances
        1024,
        // Serialize
        {{"children", [](std::ostream& os, void* data)
            {
                std::vector<EntityId>* children = static_cast<std::vector<EntityId>*>(data);
                for(EntityId child : *children)
                    write(os, child);
                write(os, EntityId(-1));
            }
        }},
        // Deserialize
        {{"children", [](std::istream& is, void* data)
            {
                std::vector<EntityId>* children = static_cast<std::vector<EntityId>*>(data);
                EntityId eid;
                read(is, eid);
                while(eid != -1)
                {
                    children->push_back(eid);
                    read(is, eid);
                }
            }
        }}
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_RELATIONSHIP_COMPONENT_H

