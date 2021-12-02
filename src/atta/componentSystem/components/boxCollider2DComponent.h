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
	/// %Component to create 2D box collider
	/** This collider will only work with 2D physics engines.
	 *
	 * TransformComponent and RigidBody2DComponent are necessary for the 
	 * entity to participate in the physics iteration.
	 */
    struct BoxCollider2DComponent final : public Component
    {
		vec2 size = { 1.0f, 1.0f };//< Box width and height
		vec2 offset = { 0.0f, 0.0f };
    };
    ATTA_REGISTER_COMPONENT(BoxCollider2DComponent)
    template<> ComponentRegistry::Description& TypedComponentRegistry<BoxCollider2DComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_BOX_COLLIDER_2D_COMPONENT_H
