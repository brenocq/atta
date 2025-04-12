//--------------------------------------------------
// Atta Component Module
// rigidBody.cpp
// Date: 2021-12-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/rigidBody.h>
#include <atta/physics/engines/bulletEngine.h>
#include <atta/physics/interface.h>

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

void RigidBody::setLinearVelocity(vec3 vel) {
    if (phy::getEngineType() == phy::Engine::BULLET) {
        std::shared_ptr<phy::BulletEngine> bEngine = std::static_pointer_cast<phy::BulletEngine>(phy::getEngine());
        bEngine->setLinearVelocity(this, vel);
    } else {
        LOG_WARN("cmp::RigidBody", "Could not execute [w]setLinearVelocity[], BULLET is not the current physics engine");
    }
}

void RigidBody::setAngularVelocity(vec3 omega) {
    if (phy::getEngineType() == phy::Engine::BULLET) {
        std::shared_ptr<phy::BulletEngine> bEngine = std::static_pointer_cast<phy::BulletEngine>(phy::getEngine());
        bEngine->setAngularVelocity(this, omega);
    } else {
        LOG_WARN("cmp::RigidBody", "Could not execute [w]setAngularVelocity[], BULLET is not the current physics engine");
    }
}

void RigidBody::applyForce(vec3 force, vec3 point) {
    if (phy::getEngineType() == phy::Engine::BULLET) {
        std::shared_ptr<phy::BulletEngine> bEngine = std::static_pointer_cast<phy::BulletEngine>(phy::getEngine());
        bEngine->applyForce(this, force, point);
    } else {
        LOG_WARN("cmp::RigidBody", "Could not execute [w]applyForce[], BULLET is not the current physics engine");
    }
}

void RigidBody::applyForceToCenter(vec3 force) {
    if (phy::getEngineType() == phy::Engine::BULLET) {
        std::shared_ptr<phy::BulletEngine> bEngine = std::static_pointer_cast<phy::BulletEngine>(phy::getEngine());
        bEngine->applyForceToCenter(this, force);
    } else {
        LOG_WARN("cmp::RigidBody", "Could not execute [w]applyForceToCenter[], BULLET is not the current physics engine");
    }
}

void RigidBody::applyTorque(vec3 torque) {
    if (phy::getEngineType() == phy::Engine::BULLET) {
        std::shared_ptr<phy::BulletEngine> bEngine = std::static_pointer_cast<phy::BulletEngine>(phy::getEngine());
        bEngine->applyTorque(this, torque);
    } else {
        LOG_WARN("cmp::RigidBody", "Could not execute [w]applyTorque[], BULLET is not the current physics engine");
    }
}

} // namespace atta::component
