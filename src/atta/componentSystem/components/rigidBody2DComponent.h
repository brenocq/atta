//--------------------------------------------------
// Atta Component System
// rigidBody2DComponent.h
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_RIGID_BODY_2D_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_RIGID_BODY_2D_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/math/vector.h>

namespace atta
{
    struct RigidBody2DComponent final : public Component
    {
		enum Type : uint32_t
		{
			DYNAMIC = 0,
			KINEMATIC,
			STATIC
		};

		Type type = DYNAMIC;
		// Rigid body state
		vec2 linearVelocity = { 0.0f, 0.0f };
		float angularVelocity = 0.0f;
		// Material properties
		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.5f;
		// Simulation properties
		float linearDamping = 0.0f;
		float angularDamping = 0.0f;
		bool allowSleep = true;
		bool awake = true;
		bool fixedRotation = false;
    };
    ATTA_REGISTER_COMPONENT(RigidBody2DComponent)
    template<> ComponentRegistry::Description& TypedComponentRegistry<RigidBody2DComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_RIGID_BODY_2D_COMPONENT_H
