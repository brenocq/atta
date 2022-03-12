//--------------------------------------------------
// Atta Physics System
// box2DEngine.h
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_BOX2D_ENGINE_H
#define ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_BOX2D_ENGINE_H
#include <atta/physicsSystem/physicsEngines/physicsEngine.h>
#include <atta/componentSystem/components/rigidBody2DComponent.h>
#include <atta/componentSystem/components/prismaticJointComponent.h>
#include <atta/componentSystem/components/revoluteJointComponent.h>
#include <atta/componentSystem/components/rigidJointComponent.h>
#include "box2d/box2d.h"

namespace atta
{
    class Box2DEngine : public PhysicsEngine
    {
    public:
        Box2DEngine();
        ~Box2DEngine();

        void start() override;
        void step(float dt) override;
        void stop() override;

        std::vector<EntityId> getEntityCollisions(EntityId eid) override;
        std::vector<EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst) override;
        bool areColliding(EntityId eid0, EntityId eid1) override;

        // RigidBody2DComponent interface
        void setTransform(RigidBody2DComponent* rb2d, vec2 position, float angle);
        void setLinearVelocity(RigidBody2DComponent* rb2d, vec2 vel);
        void setAngularVelocity(RigidBody2DComponent* rb2d, float omega);
        void applyForce(RigidBody2DComponent* rb2d, vec2 force, vec2 point, bool wake);
        void applyForceToCenter(RigidBody2DComponent* rb2d, vec2 force, bool wake);
        void applyTorque(RigidBody2DComponent* rb2d, float torque, bool wake);

    private:
        std::vector<EntityId> getAABBEntities(vec2 lower, vec2 upper);

        void createPrismaticJoint(PrismaticJointComponent* prismatic);
        void createRevoluteJoint(RevoluteJointComponent* revolute);
        void createRigidJoint(RigidJointComponent* rigid);

        std::shared_ptr<b2World> _world;
        std::unordered_map<EntityId, b2Body*> _bodies;
        std::unordered_map<RigidBody2DComponent*, EntityId> _componentToEntity;
        std::unordered_map<EntityId, std::unordered_set<EntityId>> _collisions;
    };
}

#endif// ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_BOX2D_ENGINE_H
