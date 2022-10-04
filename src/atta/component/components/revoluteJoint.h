//--------------------------------------------------
// Atta Component Module
// revoluteJoint.h
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------

#ifndef ATTA_COMPONENT_COMPONENTS_REVOLUTE_JOINT_H
#define ATTA_COMPONENT_COMPONENTS_REVOLUTE_JOINT_H
#include <atta/component/base.h>
#include <atta/component/interface.h>

namespace atta::component {
/// Revolute joint between entities
/** Each body have a anchor point on its local space. This joint can be used as a motor. */
struct RevoluteJoint : public Component {
    EntityId bodyA; ///< First attached body
    EntityId bodyB; ///< Second attached body

    vec3 anchorA = vec3(0, 0, 0); ///< BodyA's anchor on its local space
    vec3 anchorB = vec3(0, 0, 0); ///< BodyB's anchor on its local space
    vec3 axisA = vec3(0, 0, 1);   ///< Axis direction on bodyA's local coordinate system
    vec3 axisB = vec3(0, 0, 1);   ///< Axis direction on bodyB's local coordinate system

    bool enableLimits = false; ///< Enable joint limits
    float lowerAngle = 0.0f;   ///< Joint angle lower limit
    float upperAngle = 0.0f;   ///< Joint angle upper limit

    bool enableMotor = false;         ///< Enable motor
    float targetMotorVelocity = 0.0f; ///< Motor velocity
    float maxMotorForce = 100.0f;     ///< Maximum motor force
    float motorAngle = 0.0f;          ///< Motor angle

    bool shouldCollide = false; ///< If bodyA and bodyB should collide
};
ATTA_REGISTER_COMPONENT(RevoluteJoint)
template <>
ComponentDescription& TypedComponentRegistry<RevoluteJoint>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_REVOLUTE_JOINT_COMPONENT_BASE_H
