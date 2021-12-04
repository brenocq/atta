//--------------------------------------------------
// Atta Component System
// revoluteJointComponent.cpp
// Date: 2021-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/revoluteJointComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<RevoluteJointComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
        {
            "Revolute Joint",
            {
                { ComponentRegistry::AttributeType::INT32, offsetof(RevoluteJointComponent, bodyA), "bodyA" },
                { ComponentRegistry::AttributeType::INT32, offsetof(RevoluteJointComponent, bodyB), "bodyB" },
                { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJointComponent, anchorA), "anchorA" },
                { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJointComponent, anchorB), "anchorB" },
                { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJointComponent, axisRotation), "axisRotation" },
                { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJointComponent, axisZero), "axisZero" },
                { ComponentRegistry::AttributeType::BOOL, offsetof(RevoluteJointComponent, enableLimits), "enableLimits" },
                { ComponentRegistry::AttributeType::FLOAT32, offsetof(RevoluteJointComponent, lowerAngle), "lowerAngle" },
                { ComponentRegistry::AttributeType::FLOAT32, offsetof(RevoluteJointComponent, upperAngle), "upperAngle" },
                { ComponentRegistry::AttributeType::BOOL, offsetof(RevoluteJointComponent, enableMotor), "enableMotor" },
                { ComponentRegistry::AttributeType::FLOAT32, offsetof(RevoluteJointComponent, motorSpeed), "motorSpeed" },
                { ComponentRegistry::AttributeType::FLOAT32, offsetof(RevoluteJointComponent, maxMotorTorque), "maxMotorTorque" },
                { ComponentRegistry::AttributeType::BOOL, offsetof(RevoluteJointComponent, collideConnected), "collideConnected" },
            }
        };

        return desc;
    }
}
