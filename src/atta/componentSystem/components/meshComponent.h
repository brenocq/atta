//--------------------------------------------------
// Atta Component System
// meshComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
#include <atta/componentSystem/components/component.h>

namespace atta
{
    struct MeshComponent final : public TypedComponent<MeshComponent>
    {
        StringId sid;
    };

    template<> const Component::TypeDescription TypedComponent<MeshComponent>::typeDescription;
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
