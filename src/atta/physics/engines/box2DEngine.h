//--------------------------------------------------
// Atta Physics Module
// box2DEngine.h
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_PHYSICS_ENGINES_BOX2D_ENGINE_H
#define ATTA_PHYSICS_PHYSICS_ENGINES_BOX2D_ENGINE_H

#include "box2d/box2d.h"
#include <atta/component/components/prismaticJoint.h>
#include <atta/component/components/revoluteJoint.h>
#include <atta/component/components/rigidBody2D.h>
#include <atta/component/components/rigidJoint.h>
#include <atta/physics/engines/engine.h>

namespace atta::physics {

class Box2DEngine : public Engine {
  public:
    Box2DEngine();
    ~Box2DEngine();

    void start() override;
    void step(float dt) override;
    void stop() override;

    std::vector<component::EntityId> getEntityCollisions(component::EntityId eid) override;
    std::vector<component::EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst) override;
    bool areColliding(component::EntityId eid0, component::EntityId eid1) override;

    // component::RigidBody2D interface
    void setTransform(component::RigidBody2D* rb2d, vec2 position, float angle);
    void setLinearVelocity(component::RigidBody2D* rb2d, vec2 vel);
    void setAngularVelocity(component::RigidBody2D* rb2d, float omega);
    void applyForce(component::RigidBody2D* rb2d, vec2 force, vec2 point, bool wake);
    void applyForceToCenter(component::RigidBody2D* rb2d, vec2 force, bool wake);
    void applyTorque(component::RigidBody2D* rb2d, float torque, bool wake);

  private:
    std::vector<component::EntityId> getAABBEntities(vec2 lower, vec2 upper);

    void createPrismaticJoint(component::PrismaticJoint* prismatic);
    void createRevoluteJoint(component::RevoluteJoint* revolute);
    void createRigidJoint(component::RigidJoint* rigid);

    std::shared_ptr<b2World> _world;
    std::unordered_map<component::EntityId, b2Body*> _bodies;
    std::unordered_map<component::RigidBody2D*, component::EntityId> _componentToEntity;
    std::unordered_map<component::EntityId, std::unordered_set<component::EntityId>> _collisions;
};

} // namespace atta::physics

#endif // ATTA_PHYSICS_PHYSICS_ENGINES_BOX2D_ENGINE_H
