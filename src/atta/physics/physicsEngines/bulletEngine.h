//--------------------------------------------------
// Atta Physics Module
// bulletEngine.h
// Date: 2021-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_PHYSICS_ENGINES_BULLET_ENGINE_H
#define ATTA_PHYSICS_PHYSICS_ENGINES_BULLET_ENGINE_H

#include "btBulletDynamicsCommon.h"
#include <atta/component/components/prismaticJoint.h>
#include <atta/component/components/revoluteJoint.h>
#include <atta/component/components/rigidJoint.h>
#include <atta/physics/physicsEngines/physicsEngine.h>

namespace atta::physics {

class BulletEngine : public PhysicsEngine {
  public:
    BulletEngine();
    ~BulletEngine();

    void start() override;
    void step(float dt) override;
    void stop() override;

    std::vector<component::EntityId> getEntityCollisions(component::EntityId eid) override;
    std::vector<component::EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst) override;
    bool areColliding(component::EntityId eid0, component::EntityId eid1) override;

  private:
    void createRigidBody(component::EntityId entity);
    void createPrismaticJoint(component::PrismaticJoint* prismatic);
    void createRevoluteJoint(component::RevoluteJoint* revolute);
    void createRigidJoint(component::RigidJoint* rigid);

    std::shared_ptr<btDiscreteDynamicsWorld> _world;
    std::shared_ptr<btSequentialImpulseConstraintSolver> _solver;
    std::shared_ptr<btBroadphaseInterface> _broadPhase;
    std::shared_ptr<btCollisionDispatcher> _dispatcher;
    std::shared_ptr<btDefaultCollisionConfiguration> _collisionConfiguration;
    btAlignedObjectArray<btCollisionShape*> _collisionShapes;
};

} // namespace atta::physics

#endif // ATTA_PHYSICS_PHYSICS_ENGINES_BULLET_ENGINE_H
