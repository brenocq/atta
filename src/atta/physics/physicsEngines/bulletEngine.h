//--------------------------------------------------
// Atta Physics Module
// bulletEngine.h
// Date: 2021-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_PHYSICS_ENGINES_BULLET_ENGINE_H
#define ATTA_PHYSICS_PHYSICS_ENGINES_BULLET_ENGINE_H
#include "btBulletDynamicsCommon.h"
#include <atta/component/components/prismaticJointComponent.h>
#include <atta/component/components/revoluteJointComponent.h>
#include <atta/component/components/rigidJointComponent.h>
#include <atta/physics/physicsEngines/physicsEngine.h>

namespace atta::physics {
class BulletEngine : public PhysicsEngine {
  public:
    BulletEngine();
    ~BulletEngine();

    void start() override;
    void step(float dt) override;
    void stop() override;

    std::vector<EntityId> getEntityCollisions(EntityId eid) override;
    std::vector<EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst) override;
    bool areColliding(EntityId eid0, EntityId eid1) override;

  private:
    void createRigidBody(EntityId entity);
    void createPrismaticJoint(PrismaticJointComponent* prismatic);
    void createRevoluteJoint(RevoluteJointComponent* revolute);
    void createRigidJoint(RigidJointComponent* rigid);

    std::shared_ptr<btDiscreteDynamicsWorld> _world;
    std::shared_ptr<btSequentialImpulseConstraintSolver> _solver;
    std::shared_ptr<btBroadphaseInterface> _broadPhase;
    std::shared_ptr<btCollisionDispatcher> _dispatcher;
    std::shared_ptr<btDefaultCollisionConfiguration> _collisionConfiguration;
    btAlignedObjectArray<btCollisionShape*> _collisionShapes;
};
} // namespace atta::physics

#endif // ATTA_PHYSICS_PHYSICS_ENGINES_BULLET_ENGINE_H
