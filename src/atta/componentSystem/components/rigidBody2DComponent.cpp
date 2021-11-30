//--------------------------------------------------
// Atta Component System
// rigidBody2DComponent.cpp
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/rigidBody2DComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<RigidBody2DComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "RigidBody2D",
                {
                    { ComponentRegistry::AttributeType::UINT32, offsetof(RigidBody2DComponent, type), "type", {}, {}, {}, { "Dynamic", "Kinematic", "Static" } },
                    { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(RigidBody2DComponent, position), "position", {}, {}, 0.001f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(RigidBody2DComponent, angle), "angle", 0.0f, 3.1415926535f*2  },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(RigidBody2DComponent, linearVelocity), "linearVelocity", {}, {}, 0.001f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(RigidBody2DComponent, angularVelocity), "angularVelocity", {}, {}, 0.001f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(RigidBody2DComponent, linearDamping), "linearDamping", {}, {}, 0.001f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(RigidBody2DComponent, angularDamping), "angularDamping", {}, {}, 0.001f },
                    { ComponentRegistry::AttributeType::BOOL, offsetof(RigidBody2DComponent, allowSleep), "allowSleep" },
                    { ComponentRegistry::AttributeType::BOOL, offsetof(RigidBody2DComponent, awake), "awake" },
                    { ComponentRegistry::AttributeType::BOOL, offsetof(RigidBody2DComponent, fixedRotation), "fixedRotation" }
                }
            };

        return desc;
    }
}
