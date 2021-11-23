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
        MeshComponent();
        StringId sid;
    };
    ATTA_REGISTER_COMPONENT(MeshComponent)
    template<> ComponentRegistry::Description& TypedComponentRegistry<MeshComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
