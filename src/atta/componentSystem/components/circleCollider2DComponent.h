//--------------------------------------------------
// Atta Component System
// circleCollider2DComponent.h
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_CIRCLE_COLLIDER_2D_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_CIRCLE_COLLIDER_2D_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/math/vector.h>

namespace atta
{
	/// %Component to create 2D circle collider
	/** This collider will only work with 2D physics engines.
	 *
	 * TransformComponent and RigidBody2DComponent are necessary for the 
	 * entity to participate in the physics iteration.
	 */
    struct CircleCollider2DComponent final : public Component
    {
		float radius = 0.5f;
		vec2 offset = { 0.0f, 0.0f };
    };
    ATTA_REGISTER_COMPONENT(CircleCollider2DComponent)
    template<> ComponentRegistry::Description& TypedComponentRegistry<CircleCollider2DComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_CIRCLE_COLLIDER_2D_COMPONENT_H
