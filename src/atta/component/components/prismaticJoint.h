//--------------------------------------------------
// Atta Component Module
// prismaticJoint.h
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_PRISMATIC_JOINT_H
#define ATTA_COMPONENT_COMPONENTS_PRISMATIC_JOINT_H

#include <atta/component/base.h>
#include <atta/component/interface.h>

namespace atta::component {

/// Prismatic joint between entities
/** A prismatic joint has only one degree of freedom, that is translation
 * along an axis. The bodies don't rotate relative to each other*/
struct PrismaticJoint : public Component {
    EntityId bodyA; ///< First attached body
    EntityId bodyB; ///< Second attached body

    vec3 anchorA = vec3(0, 0, 0); ///< BodyA's anchor on its local space
    vec3 anchorB = vec3(0, 0, 0); ///< BodyB's anchor on its local space
    vec3 axisA = vec3(0, 0, 1);   ///< Axis direction on bodyA's local coordinate system
    vec3 axisB = vec3(0, 0, 1);   ///< Axis direction on bodyB's local coordinate system

    bool enableLimits = false; ///< Enable joint limits
    float lowerLimit = 1.0f;   ///< Joint lower limit
    float upperLimit = 2.0f;   ///< Joint upper limit

    bool enableMotor = false;         ///< Enable motor
    float targetMotorVelocity = 0.0f; ///< Target motor velocity
    float maxMotorForce = 100.0f;     ///< Maximum motor force
    float motorPosition = 0.0f;       ///< Current motor position

    bool shouldCollide = false; ///< If bodyA and bodyB should collide
};
ATTA_REGISTER_COMPONENT(PrismaticJoint)
template <>
ComponentDescription& TypedRegistry<PrismaticJoint>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_PRISMATIC_JOINT_COMPONENT_BASE_H
