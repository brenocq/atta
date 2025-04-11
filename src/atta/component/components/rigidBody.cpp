//--------------------------------------------------
// Atta Component Module
// rigidBody.cpp
// Date: 2021-12-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/rigidBody.h>

namespace atta::component {
template <>
ComponentDescription& TypedComponentRegistry<RigidBody>::getDescription() {
    static ComponentDescription desc = {"Rigid Body",
                                        {{AttributeType::UINT32, offsetof(RigidBody, type), "type", {}, {}, {}, {"DYNAMIC", "KINEMATIC", "STATIC"}},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(RigidBody, linearVelocity), "linearVelocity", {}, {}, 0.001f},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(RigidBody, angularVelocity), "angularVelocity", {}, {}, 0.001f},
                                         {AttributeType::FLOAT32, offsetof(RigidBody, mass), "mass", 0.0f, 100.0f, 0.001f},
                                         {AttributeType::FLOAT32, offsetof(RigidBody, friction), "friction", 0.0f, 1.0f, 0.001f},
                                         {AttributeType::FLOAT32, offsetof(RigidBody, restitution), "restitution", 0.0f, 1.0f, 0.001f},
                                         {AttributeType::FLOAT32, offsetof(RigidBody, linearDamping), "linearDamping", {}, {}, 0.001f},
                                         {AttributeType::FLOAT32, offsetof(RigidBody, angularDamping), "angularDamping", {}, {}, 0.001f},
                                         {AttributeType::BOOL, offsetof(RigidBody, allowSleep), "allowSleep"},
                                         {AttributeType::BOOL, offsetof(RigidBody, awake), "awake"},
                                         {AttributeType::UINT8, offsetof(RigidBody, constraints), "constraints"}}};

    return desc;
}

} // namespace atta::component
