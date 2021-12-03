//--------------------------------------------------
// Atta Component System
// revoluteJointComponent.h
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_REVOLUTE_JOINT_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_REVOLUTE_JOINT_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/componentSystem/base.h>

namespace atta
{
	/// Revolute joint between entities
	/** Each body have a anchor point on its local space. This joint can be used as a motor. */
    struct RevoluteJointComponent : public Component 
    {
		EntityId bodyA;///< First attached body
		EntityId bodyB;///< Second attached body

		vec3 anchorA;///< BodyA's anchor on its local space
		vec3 anchorB;///< BodyB's anchor on its local space
		vec3 axisRotation;///< Revolute joint axis of rotation on bodyA's local space
		vec3 axisZero;///< Direction where the angle is zero

		bool enableLimits;///< Enable joint limits

		/** The angle is acording to the vector between be anchorA and */
		float lowerAngle;///< Minimum joint angle
		float upperAngle;///< Minimum joint angle

		bool enableMotor;///< Enable motor
		float motorSpeed;///< Motor speed
		float motorToque;///< Motor torque
    };
    ATTA_REGISTER_COMPONENT(RevoluteJointComponent)
    template<> ComponentRegistry::Description& TypedComponentRegistry<RevoluteJointComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_REVOLUTE_JOINT_COMPONENT_BASE_H
