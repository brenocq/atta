//--------------------------------------------------
// Atta Component System
// boxCollider2DComponent.h
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_BOX_COLLIDER_2D_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_BOX_COLLIDER_2D_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/math/vector.h>

namespace atta
{
    struct BoxCollider2DComponent final : public Component
    {
		vec2 size = { 1.0f, 1.0f };
		vec2 offset = { 0.0f, 0.0f };
    };
    ATTA_REGISTER_COMPONENT(BoxCollider2DComponent)
    template<> ComponentRegistry::Description& TypedComponentRegistry<BoxCollider2DComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_BOX_COLLIDER_2D_COMPONENT_H
