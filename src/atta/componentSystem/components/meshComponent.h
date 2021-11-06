//--------------------------------------------------
// Atta Component System
// meshComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/stringId.h>

namespace atta
{
    struct MeshComponent final : public Component
    {
        StringId sid;
    };
    ATTA_REGISTER_COMPONENT(MeshComponent)

    template<>
    inline ComponentRegistry::Description TypedComponentRegistry<MeshComponent>::description = 
    {
        "Mesh",
        {
            { ComponentRegistry::AttributeType::STRINGID, offsetof(MeshComponent, sid), "sid", {}, {}, {}, {} },
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
