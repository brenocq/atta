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

    vec3 anchorA;              ///< BodyA's anchor on its local space
    vec3 anchorB;              ///< BodyB's anchor on its local space
    vec3 axis = vec3(0, 0, 1); ///< Slider direction on bodyA's local coordinate system

    bool enableLimits = false;     ///< Enable joint limits
    float lowerTranslation = 1.0f; ///< Joint translation lower limit
    float upperTranslation = 2.0f; ///< Joint translation upper limit

    bool enableMotor = false;       ///< Enable motor
    float motorSpeed = 0.0f;        ///< Motor speed
    float maxMotorForce = 10000.0f; ///< Maximum motor force

    bool collideConnected = false; ///< If bodyA and bodyB should collide
};
ATTA_REGISTER_COMPONENT(PrismaticJoint)
template <>
ComponentDescription& TypedComponentRegistry<PrismaticJoint>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_PRISMATIC_JOINT_COMPONENT_BASE_H
