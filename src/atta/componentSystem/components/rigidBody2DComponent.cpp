//--------------------------------------------------
// Atta Component System
// rigidBody2DComponent.cpp
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/rigidBody2DComponent.h>
#include <atta/physicsSystem/physicsManager.h>
#include <atta/physicsSystem/physicsEngines/box2DEngine.h>

namespace atta
{
    template<>
    ComponentDescription& TypedComponentRegistry<RigidBody2DComponent>::getDescription()
    {
        static ComponentDescription desc = 
            {
                "Rigid Body 2D",
                {
                    { AttributeType::UINT32, offsetof(RigidBody2DComponent, type), "type", {}, {}, {}, 
						{ "Dynamic", "Kinematic", "Static" } },
                    { AttributeType::FLOAT32, offsetof(RigidBody2DComponent, linearVelocity), "linearVelocity", {}, {}, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBody2DComponent, angularVelocity), "angularVelocity", {}, {}, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBody2DComponent, mass), "mass", 0.0f, 100.0f, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBody2DComponent, friction), "friction", 0.0f, 1.0f, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBody2DComponent, restitution), "restitution", 0.0f, 1.0f, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBody2DComponent, linearDamping), "linearDamping", {}, {}, 0.001f },
                    { AttributeType::FLOAT32, offsetof(RigidBody2DComponent, angularDamping), "angularDamping", {}, {}, 0.001f },
                    { AttributeType::BOOL, offsetof(RigidBody2DComponent, allowSleep), "allowSleep" },
                    { AttributeType::BOOL, offsetof(RigidBody2DComponent, awake), "awake" },
                    { AttributeType::BOOL, offsetof(RigidBody2DComponent, fixedRotation), "fixedRotation" }
                }
            };

        return desc;
    }

    void RigidBody2DComponent::setTransform(vec2 position, float angle)
    {
        if(PhysicsManager::getSelectedEngine() == PhysicsEngine::BOX2D_ENGINE)
        {
            std::shared_ptr<Box2DEngine> b2Engine = std::static_pointer_cast<Box2DEngine>(PhysicsManager::getPhysicsEngine());
            b2Engine->setTransform(this, position, angle);
        }
        else
            LOG_WARN("RigidBody2DComponent", "Could not execute [w]setTransform[], box2D is not the current physics engine.");
    }

    void RigidBody2DComponent::setLinearVelocity(vec2 vel)
    {
        if(PhysicsManager::getSelectedEngine() == PhysicsEngine::BOX2D_ENGINE)
        {
            std::shared_ptr<Box2DEngine> b2Engine = std::static_pointer_cast<Box2DEngine>(PhysicsManager::getPhysicsEngine());
            b2Engine->setLinearVelocity(this, vel);
        }
        else
            LOG_WARN("RigidBody2DComponent", "Could not execute [w]setLinearVelocity[], box2D is not the current physics engine.");
    }

    void RigidBody2DComponent::setAngularVelocity(float omega)
    {
        if(PhysicsManager::getSelectedEngine() == PhysicsEngine::BOX2D_ENGINE)
        {
            std::shared_ptr<Box2DEngine> b2Engine = std::static_pointer_cast<Box2DEngine>(PhysicsManager::getPhysicsEngine());
            b2Engine->setAngularVelocity(this, omega);
        }
        else
            LOG_WARN("RigidBody2DComponent", "Could not execute [w]setAngularVelocity[], box2D is not the current physics engine.");
    }

    void RigidBody2DComponent::applyForce(vec2 force, vec2 point, bool wake)
    {
        if(PhysicsManager::getSelectedEngine() == PhysicsEngine::BOX2D_ENGINE)
        {
            std::shared_ptr<Box2DEngine> b2Engine = std::static_pointer_cast<Box2DEngine>(PhysicsManager::getPhysicsEngine());
            b2Engine->applyForce(this, force, point, wake);
        }
        else
            LOG_WARN("RigidBody2DComponent", "Could not execute [w]applyForce[], box2D is not the current physics engine.");
    }

    void RigidBody2DComponent::applyForceToCenter(vec2 force, bool wake)
    {
        if(PhysicsManager::getSelectedEngine() == PhysicsEngine::BOX2D_ENGINE)
        {
            std::shared_ptr<Box2DEngine> b2Engine = std::static_pointer_cast<Box2DEngine>(PhysicsManager::getPhysicsEngine());
            b2Engine->applyForceToCenter(this, force, wake);
        }
        else
            LOG_WARN("RigidBody2DComponent", "Could not execute [w]applyForceToCenter[], box2D is not the current physics engine.");
    }
    
    void RigidBody2DComponent::applyTorque(float torque, bool wake)
    {
        if(PhysicsManager::getSelectedEngine() == PhysicsEngine::BOX2D_ENGINE)
        {
            std::shared_ptr<Box2DEngine> b2Engine = std::static_pointer_cast<Box2DEngine>(PhysicsManager::getPhysicsEngine());
            b2Engine->applyTorque(this, torque, wake);
        }
        else
            LOG_WARN("RigidBody2DComponent", "Could not execute [w]applyTorque[], box2D is not the current physics engine.");
    }
}
