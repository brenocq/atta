//--------------------------------------------------
// Atta Component Module
// revoluteJointComponent.h
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_REVOLUTE_JOINT_COMPONENT_H
#define ATTA_COMPONENT_COMPONENTS_REVOLUTE_JOINT_COMPONENT_H
#include <atta/component/base.h>
#include <atta/component/componentManager.h>

namespace atta::component {
/// Revolute joint between entities
/** Each body have a anchor point on its local space. This joint can be used as a motor. */
struct RevoluteJointComponent : public Component {
    EntityId bodyA; ///< First attached body
    EntityId bodyB; ///< Second attached body

    vec3 anchorA;      ///< BodyA's anchor on its local space
    vec3 anchorB;      ///< BodyB's anchor on its local space
    vec3 axisRotation; ///< Revolute joint axis of rotation on bodyA's local space
    /** This axis must be perpendicular to the axisRotation to create the joint base */
    vec3 axisZero; ///< Direction where the angle is zero

    bool enableLimits; ///< Enable joint limits
    float lowerAngle;  ///< Joint angle lower limit
    float upperAngle;  ///< Joint angle upper limit

    bool enableMotor;     ///< Enable motor
    float motorSpeed;     ///< Motor speed
    float maxMotorTorque; ///< Maximum motor torque

    bool collideConnected = false; ///< If bodyA and bodyB should collide
};
ATTA_REGISTER_COMPONENT(RevoluteJointComponent)
template <>
ComponentDescription& TypedComponentRegistry<RevoluteJointComponent>::getDescription();
} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_REVOLUTE_JOINT_COMPONENT_BASE_H
