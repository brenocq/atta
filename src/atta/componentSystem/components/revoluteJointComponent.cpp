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
    ComponentDescription& TypedComponentRegistry<RevoluteJointComponent>::getDescription()
    {
        static ComponentDescription desc = 
        {
            "Revolute Joint",
            {
                { AttributeType::INT32, offsetof(RevoluteJointComponent, bodyA), "bodyA" },
                { AttributeType::INT32, offsetof(RevoluteJointComponent, bodyB), "bodyB" },
                { AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJointComponent, anchorA), "anchorA" },
                { AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJointComponent, anchorB), "anchorB" },
                { AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJointComponent, axisRotation), "axisRotation" },
                { AttributeType::VECTOR_FLOAT32, offsetof(RevoluteJointComponent, axisZero), "axisZero" },
                { AttributeType::BOOL, offsetof(RevoluteJointComponent, enableLimits), "enableLimits" },
                { AttributeType::FLOAT32, offsetof(RevoluteJointComponent, lowerAngle), "lowerAngle" },
                { AttributeType::FLOAT32, offsetof(RevoluteJointComponent, upperAngle), "upperAngle" },
                { AttributeType::BOOL, offsetof(RevoluteJointComponent, enableMotor), "enableMotor" },
                { AttributeType::FLOAT32, offsetof(RevoluteJointComponent, motorSpeed), "motorSpeed" },
                { AttributeType::FLOAT32, offsetof(RevoluteJointComponent, maxMotorTorque), "maxMotorTorque" },
                { AttributeType::BOOL, offsetof(RevoluteJointComponent, collideConnected), "collideConnected" },
            }
        };

        return desc;
    }
}
