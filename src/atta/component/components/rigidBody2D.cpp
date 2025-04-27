//--------------------------------------------------
// Atta Component Module
// rigidBody2D.cpp
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/rigidBody2D.h>
#include <atta/physics/engines/box2DEngine.h>
#include <atta/physics/interface.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<RigidBody2D>::getDescription() {
    static ComponentDescription desc = {
        "Rigid Body 2D",
        {
            {AttributeType::UINT32, offsetof(RigidBody2D, type), "type", {}, {}, {}, {"DYNAMIC", "KINEMATIC", "STATIC"}},
            {AttributeType::VECTOR_FLOAT32, offsetof(RigidBody2D, linearVelocity), "linearVelocity", {}, {}, 0.001f},
            {AttributeType::FLOAT32, offsetof(RigidBody2D, angularVelocity), "angularVelocity", {}, {}, 0.001f},
            {AttributeType::FLOAT32, offsetof(RigidBody2D, mass), "mass", 0.0f, 100.0f, 0.001f},
            {AttributeType::FLOAT32, offsetof(RigidBody2D, friction), "friction", 0.0f, 1.0f, 0.001f},
            {AttributeType::FLOAT32, offsetof(RigidBody2D, restitution), "restitution", 0.0f, 1.0f, 0.001f},
            {AttributeType::FLOAT32, offsetof(RigidBody2D, linearDamping), "linearDamping", {}, {}, 0.001f},
            {AttributeType::FLOAT32, offsetof(RigidBody2D, angularDamping), "angularDamping", {}, {}, 0.001f},
            {AttributeType::BOOL, offsetof(RigidBody2D, allowSleep), "allowSleep"},
            {AttributeType::BOOL, offsetof(RigidBody2D, awake), "awake"},
            {AttributeType::BOOL, offsetof(RigidBody2D, fixedRotation), "fixedRotation"},
            {AttributeType::BOOL, offsetof(RigidBody2D, groundFriction), "groundFriction"},
        }};

    return desc;
}

void RigidBody2D::setTransform(vec2 position, float angle) {
    if (physics::getEngineType() == physics::Engine::BOX2D) {
        std::shared_ptr<physics::Box2DEngine> b2Engine = std::static_pointer_cast<physics::Box2DEngine>(physics::getEngine());
        b2Engine->setTransform(this, position, angle);
    } else
        LOG_WARN("component::RigidBody2D", "Could not execute [w]setTransform[], box2D is not the current physics engine.");
}

void RigidBody2D::setLinearVelocity(vec2 vel) {
    if (physics::getEngineType() == physics::Engine::BOX2D) {
        std::shared_ptr<physics::Box2DEngine> b2Engine = std::static_pointer_cast<physics::Box2DEngine>(physics::getEngine());
        b2Engine->setLinearVelocity(this, vel);
    } else
        LOG_WARN("component::RigidBody2D", "Could not execute [w]setLinearVelocity[], box2D is not the current physics engine.");
}

void RigidBody2D::setAngularVelocity(float omega) {
    if (physics::getEngineType() == physics::Engine::BOX2D) {
        std::shared_ptr<physics::Box2DEngine> b2Engine = std::static_pointer_cast<physics::Box2DEngine>(physics::getEngine());
        b2Engine->setAngularVelocity(this, omega);
    } else
        LOG_WARN("component::RigidBody2D", "Could not execute [w]setAngularVelocity[], box2D is not the current physics engine.");
}

void RigidBody2D::applyForce(vec2 force, vec2 point, bool wake) {
    if (physics::getEngineType() == physics::Engine::BOX2D) {
        std::shared_ptr<physics::Box2DEngine> b2Engine = std::static_pointer_cast<physics::Box2DEngine>(physics::getEngine());
        b2Engine->applyForce(this, force, point, wake);
    } else
        LOG_WARN("component::RigidBody2D", "Could not execute [w]applyForce[], box2D is not the current physics engine.");
}

void RigidBody2D::applyForceToCenter(vec2 force, bool wake) {
    if (physics::getEngineType() == physics::Engine::BOX2D) {
        std::shared_ptr<physics::Box2DEngine> b2Engine = std::static_pointer_cast<physics::Box2DEngine>(physics::getEngine());
        b2Engine->applyForceToCenter(this, force, wake);
    } else
        LOG_WARN("component::RigidBody2D", "Could not execute [w]applyForceToCenter[], box2D is not the current physics engine.");
}

void RigidBody2D::applyTorque(float torque, bool wake) {
    if (physics::getEngineType() == physics::Engine::BOX2D) {
        std::shared_ptr<physics::Box2DEngine> b2Engine = std::static_pointer_cast<physics::Box2DEngine>(physics::getEngine());
        b2Engine->applyTorque(this, torque, wake);
    } else
        LOG_WARN("component::RigidBody2D", "Could not execute [w]applyTorque[], box2D is not the current physics engine.");
}

} // namespace atta::component
