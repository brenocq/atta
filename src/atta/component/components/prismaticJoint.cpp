//--------------------------------------------------
// Atta Component Module
// prismaticJointComponent.cpp
// Date: 2021-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/prismaticJointComponent.h>

namespace atta::component {
template <>
ComponentDescription& TypedComponentRegistry<PrismaticJointComponent>::getDescription() {
    static ComponentDescription desc = {"Prismatic Joint",
                                        {{AttributeType::INT32, offsetof(PrismaticJointComponent, bodyA), "bodyA"},
                                         {AttributeType::INT32, offsetof(PrismaticJointComponent, bodyB), "bodyB"},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJointComponent, anchorA), "anchorA"},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJointComponent, anchorB), "anchorB"},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJointComponent, axis), "axis"},
                                         {AttributeType::BOOL, offsetof(PrismaticJointComponent, enableLimits), "enableLimits"},
                                         {AttributeType::FLOAT32, offsetof(PrismaticJointComponent, lowerTranslation), "lowerTranslation"},
                                         {AttributeType::FLOAT32, offsetof(PrismaticJointComponent, upperTranslation), "upperTranslation"},
                                         {AttributeType::BOOL, offsetof(PrismaticJointComponent, enableMotor), "enableMotor"},
                                         {AttributeType::FLOAT32, offsetof(PrismaticJointComponent, motorSpeed), "motorSpeed"},
                                         {AttributeType::FLOAT32, offsetof(PrismaticJointComponent, maxMotorForce), "maxMotorForce"},
                                         {AttributeType::BOOL, offsetof(PrismaticJointComponent, collideConnected), "collideConnected"}}};

    return desc;
}
} // namespace atta::component
