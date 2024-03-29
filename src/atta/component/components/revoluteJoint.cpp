//--------------------------------------------------
// Atta Component Module
// revoluteJoint.cpp
// Date: 2021-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/revoluteJoint.h>

namespace atta::component {
template <>
ComponentDescription& TypedComponentRegistry<RevoluteJoint>::getDescription() {
    static ComponentDescription desc = {
        "Revolute Joint",
        {
            {AttributeType::INT32, offsetof(RevoluteJoint, bodyA), "bodyA"},
            {AttributeType::INT32, offsetof(RevoluteJoint, bodyB), "bodyB"},
            {AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJoint, anchorA), "anchorA"},
            {AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJoint, anchorB), "anchorB"},
            {AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJoint, axisA), "axisA"},
            {AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJoint, axisB), "axisB"},
            {AttributeType::BOOL, offsetof(RevoluteJoint, enableLimits), "enableLimits"},
            {AttributeType::FLOAT32, offsetof(RevoluteJoint, lowerAngle), "lowerAngle"},
            {AttributeType::FLOAT32, offsetof(RevoluteJoint, upperAngle), "upperAngle"},
            {AttributeType::BOOL, offsetof(RevoluteJoint, enableMotor), "enableMotor"},
            {AttributeType::FLOAT32, offsetof(RevoluteJoint, targetMotorVelocity), "targetMotorVelocity"},
            {AttributeType::FLOAT32, offsetof(RevoluteJoint, maxMotorForce), "maxMotorForce"},
            {AttributeType::FLOAT32, offsetof(RevoluteJoint, motorAngle), "motorAngle"},
            {AttributeType::BOOL, offsetof(RevoluteJoint, shouldCollide), "shouldCollide"},
        },
    };

    return desc;
}

} // namespace atta::component
