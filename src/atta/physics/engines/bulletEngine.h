//--------------------------------------------------
// Atta Physics Module
// bulletEngine.h
// Date: 2021-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_ENGINES_BULLET_ENGINE_H
#define ATTA_PHYSICS_ENGINES_BULLET_ENGINE_H

#include "btBulletDynamicsCommon.h"
#include <atta/component/components/prismaticJoint.h>
#include <atta/component/components/revoluteJoint.h>
#include <atta/component/components/rigidJoint.h>
#include <atta/physics/engines/engine.h>

namespace atta::physics {

class BulletEngine : public Engine {
  public:
    BulletEngine();
    ~BulletEngine();

    void start() override;
    void step(float dt) override;
    void stop() override;

    std::vector<component::Entity> getEntityCollisions(component::Entity entity) override;
    std::vector<RayCastHit> rayCast(vec3 begin, vec3 end, bool onlyFirst) override;
    bool areColliding(component::Entity entity0, component::Entity entity1) override;

    void updateGravity() override;

    unsigned getNumSubSteps() const;
    void setNumSubSteps(unsigned numSubSteps);
    bool getShowAabb() const;
    void setShowAabb(bool showAabb);
    btRigidBody* getBulletRigidBody(component::Entity entity);
    bnd3 getAabb(component::Entity entity);
    const std::unordered_map<component::Entity, std::unordered_map<component::Entity, btPersistentManifold*>>& getCollisions();

  private:
    void createRigidBody(component::Entity entity);
    void createPrismaticJoint(component::PrismaticJoint* prismatic);
    void createRevoluteJoint(component::RevoluteJoint* revolute);
    void createRigidJoint(component::RigidJoint* rigid);

    static void collisionStarted(btPersistentManifold* const& manifold);
    static void collisionEnded(btPersistentManifold* const& manifold);

    unsigned _numSubSteps; ///< Number of physics sub steps for each simulation step

    // World configutation
    std::shared_ptr<btDiscreteDynamicsWorld> _world;
    std::shared_ptr<btSequentialImpulseConstraintSolver> _solver;
    std::shared_ptr<btBroadphaseInterface> _broadPhase;
    std::shared_ptr<btCollisionDispatcher> _dispatcher;
    std::shared_ptr<btDefaultCollisionConfiguration> _collisionConfiguration;
    btAlignedObjectArray<btCollisionShape*> _collisionShapes;

    // World data
    std::unordered_map<component::Entity, btRigidBody*> _entityToBody;
    std::unordered_map<btRigidBody*, component::Entity> _bodyToEntity;
    std::unordered_map<component::Entity, std::unordered_map<component::Entity, btPersistentManifold*>> _collisions;
    std::unordered_map<component::Entity, std::vector<component::Entity>> _connectedEntities; ///< Which entities are connect by joints

    /// Show broad phase aabb
    bool _showAabb;
};

} // namespace atta::physics

#endif // ATTA_PHYSICS_ENGINES_BULLET_ENGINE_H
