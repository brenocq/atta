//--------------------------------------------------
// Atta Component Module
// prismaticJoint.cpp
// Date: 2021-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/prismaticJoint.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<PrismaticJoint>::getDescription() {
    static ComponentDescription desc = {
        "Prismatic Joint",
        {
            {AttributeType::INT32, offsetof(PrismaticJoint, bodyA), "bodyA"},
            {AttributeType::INT32, offsetof(PrismaticJoint, bodyB), "bodyB"},
            {AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJoint, anchorA), "anchorA"},
            {AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJoint, anchorB), "anchorB"},
            {AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJoint, axisA), "axisA"},
            {AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJoint, axisB), "axisB"},
            {AttributeType::BOOL, offsetof(PrismaticJoint, enableLimits), "enableLimits"},
            {AttributeType::FLOAT32, offsetof(PrismaticJoint, lowerLimit), "lowerLimit"},
            {AttributeType::FLOAT32, offsetof(PrismaticJoint, upperLimit), "upperLimit"},
            {AttributeType::BOOL, offsetof(PrismaticJoint, enableMotor), "enableMotor"},
            {AttributeType::FLOAT32, offsetof(PrismaticJoint, targetMotorVelocity), "targetMotorVelocity"},
            {AttributeType::FLOAT32, offsetof(PrismaticJoint, maxMotorForce), "maxMotorForce"},
            {AttributeType::FLOAT32, offsetof(PrismaticJoint, motorPosition), "motorPosition"},
            {AttributeType::BOOL, offsetof(PrismaticJoint, shouldCollide), "shouldCollide"},
        },
    };
    return desc;
}

} // namespace atta::component
