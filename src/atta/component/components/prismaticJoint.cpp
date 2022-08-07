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
    static ComponentDescription desc = {"Prismatic Joint",
                                        {{AttributeType::INT32, offsetof(PrismaticJoint, bodyA), "bodyA"},
                                         {AttributeType::INT32, offsetof(PrismaticJoint, bodyB), "bodyB"},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJoint, anchorA), "anchorA"},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJoint, anchorB), "anchorB"},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJoint, axis), "axis"},
                                         {AttributeType::BOOL, offsetof(PrismaticJoint, enableLimits), "enableLimits"},
                                         {AttributeType::FLOAT32, offsetof(PrismaticJoint, lowerTranslation), "lowerTranslation"},
                                         {AttributeType::FLOAT32, offsetof(PrismaticJoint, upperTranslation), "upperTranslation"},
                                         {AttributeType::BOOL, offsetof(PrismaticJoint, enableMotor), "enableMotor"},
                                         {AttributeType::FLOAT32, offsetof(PrismaticJoint, motorSpeed), "motorSpeed"},
                                         {AttributeType::FLOAT32, offsetof(PrismaticJoint, maxMotorForce), "maxMotorForce"},
                                         {AttributeType::BOOL, offsetof(PrismaticJoint, collideConnected), "collideConnected"}}};

    return desc;
}

} // namespace atta::component
