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
                "Rigid Body 2D",
                {
                    { ComponentRegistry::AttributeType::UINT32, offsetof(RigidBody2DComponent, type), "type", {}, {}, {}, 
						{ "Dynamic", "Kinematic", "Static" } },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(RigidBody2DComponent, linearVelocity), "linearVelocity", {}, {}, 0.001f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(RigidBody2DComponent, angularVelocity), "angularVelocity", {}, {}, 0.001f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(RigidBody2DComponent, mass), "mass", 0.0f, 100.0f, 0.001f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(RigidBody2DComponent, friction), "friction", 0.0f, 1.0f, 0.001f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(RigidBody2DComponent, restitution), "restitution", 0.0f, 1.0f, 0.001f },
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
