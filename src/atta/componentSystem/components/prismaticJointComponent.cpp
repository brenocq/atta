//--------------------------------------------------
// Atta Component System
// prismaticJointComponent.cpp
// Date: 2021-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/prismaticJointComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<PrismaticJointComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
        {
            "Prismatic Joint",
            {
                { ComponentRegistry::AttributeType::INT32, offsetof(PrismaticJointComponent, bodyA), "bodyA" },
                { ComponentRegistry::AttributeType::INT32, offsetof(PrismaticJointComponent, bodyB), "bodyB" },
                { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJointComponent, anchorA), "anchorA" },
                { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJointComponent, anchorB), "anchorB" },
                { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(PrismaticJointComponent, axis), "axis" },
                { ComponentRegistry::AttributeType::BOOL, offsetof(PrismaticJointComponent, enableLimits), "enableLimits" },
                { ComponentRegistry::AttributeType::FLOAT32, offsetof(PrismaticJointComponent, lowerTranslation), "lowerTranslation" },
                { ComponentRegistry::AttributeType::FLOAT32, offsetof(PrismaticJointComponent, upperTranslation), "upperTranslation" },
                { ComponentRegistry::AttributeType::BOOL, offsetof(PrismaticJointComponent, enableMotor), "enableMotor" },
                { ComponentRegistry::AttributeType::FLOAT32, offsetof(PrismaticJointComponent, motorSpeed), "motorSpeed" },
                { ComponentRegistry::AttributeType::FLOAT32, offsetof(PrismaticJointComponent, maxMotorForce), "maxMotorForce" },
                { ComponentRegistry::AttributeType::BOOL, offsetof(PrismaticJointComponent, collideConnected), "collideConnected" }
            }
        };

        return desc;
    }
}
