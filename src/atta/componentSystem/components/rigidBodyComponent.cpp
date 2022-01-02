//--------------------------------------------------
// Atta Component System
// rigidBodyComponent.cpp
// Date: 2021-12-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/rigidBodyComponent.h>

namespace atta
{
    template<>
    ComponentDescription& TypedComponentRegistry<RigidBodyComponent>::getDescription()
    {
        static ComponentDescription desc = 
            {
                "Rigid Body",
                {
                    { AttributeType::UINT32, offsetof(RigidBodyComponent, type), "type", {}, {}, {}, 
						{ "Dynamic", "Kinematic", "Static" } },
                    { AttributeType::FLOAT32, offsetof(RigidBodyComponent, linearVelocity), "linearVelocity", {}, {}, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBodyComponent, angularVelocity), "angularVelocity", {}, {}, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBodyComponent, mass), "mass", 0.0f, 100.0f, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBodyComponent, friction), "friction", 0.0f, 1.0f, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBodyComponent, restitution), "restitution", 0.0f, 1.0f, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBodyComponent, linearDamping), "linearDamping", {}, {}, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBodyComponent, angularDamping), "angularDamping", {}, {}, 0.001f },
                    { AttributeType::BOOL, offsetof(RigidBodyComponent, allowSleep), "allowSleep" },
                    { AttributeType::BOOL, offsetof(RigidBodyComponent, awake), "awake" },
                    { AttributeType::UINT8, offsetof(RigidBodyComponent, constraints), "constraints" }
                }
            };

        return desc;
    }
}
