//--------------------------------------------------
// Atta Physics Module
// bulletEngine.cpp
// Date: 2021-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/boxCollider.h>
#include <atta/component/components/rigidBody.h>
#include <atta/component/components/sphereCollider.h>
#include <atta/component/components/transform.h>
#include <atta/physics/physicsEngines/bulletEngine.h>

namespace atta::physics {

BulletEngine::BulletEngine() : PhysicsEngine(PhysicsEngine::BULLET_ENGINE) {}

BulletEngine::~BulletEngine() {
    if (_running)
        stop();
}

void BulletEngine::start() {
    _running = true;

    _collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
    _dispatcher = std::make_shared<btCollisionDispatcher>(_collisionConfiguration.get());
    _broadPhase = std::make_shared<btDbvtBroadphase>();
    _solver = std::make_shared<btSequentialImpulseConstraintSolver>();

    _world = std::make_shared<btDiscreteDynamicsWorld>(_dispatcher.get(), _broadPhase.get(), _solver.get(), _collisionConfiguration.get());
    _world->setGravity(btVector3(0, 0, -10));

    std::vector<component::EntityId> entities = component::getNoPrototypeView();
    //---------- Create rigid bodies ----------//
    for (component::EntityId entity : entities)
        createRigidBody(entity);

    //---------- Create joints ----------//
    for (component::EntityId entity : entities) {
        auto prismatic = component::getComponent<component::PrismaticJoint>(entity);
        auto revolute = component::getComponent<component::RevoluteJoint>(entity);
        auto rigid = component::getComponent<component::RigidJoint>(entity);

        if (prismatic)
            createPrismaticJoint(prismatic);
        if (revolute)
            createRevoluteJoint(revolute);
        if (rigid)
            createRigidJoint(rigid);
    }
}

void BulletEngine::step(float dt) {
    for (int j = _world->getNumCollisionObjects() - 1; j >= 0; j--) {
        btCollisionObject* obj = _world->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        component::EntityId eid = static_cast<uint8_t*>(obj->getUserPointer()) - static_cast<uint8_t*>(0);

        btTransform trans;
        if (body && body->getMotionState())
            body->getMotionState()->getWorldTransform(trans);
        else
            trans = obj->getWorldTransform();

        // Check if object was translated
        auto t = component::getComponent<component::Transform>(eid);

        // Get atta world position and orientation
        vec3 position, scale;
        quat orientation;
        mat4 m = t->getWorldTransform(eid);
        m.getPosOriScale(position, orientation, scale);

        // Check bullet position and orientation
        float yaw, pitch, roll;
        trans.getRotation().getEulerZYX(yaw, pitch, roll);
        quat physicsOrientation({-roll, -pitch, -yaw});
        vec3 physicsPosition = {trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()};
        if (position != physicsPosition || orientation != physicsOrientation) {
            // if(position != physicsPosition)
            //     LOG_DEBUG("BulletEngine", "Different positions for $0: $1 != $2", eid, position, physicsPosition);
            // if(orientation != physicsOrientation)
            //     LOG_DEBUG("BulletEngine", "Different orientations for $0: $1 != $2", eid, orientation, physicsOrientation);

            trans =
                btTransform(btQuaternion(orientation.i, orientation.j, orientation.k, orientation.r), btVector3(position.x, position.y, position.z));

            body->setWorldTransform(trans);
        }
    }

    _world->stepSimulation(dt, 4 + 1, dt / 4);

    for (int j = _world->getNumCollisionObjects() - 1; j >= 0; j--) {
        btCollisionObject* obj = _world->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
            body->getMotionState()->getWorldTransform(trans);
        else
            trans = obj->getWorldTransform();

        // Update entity with data changed by engine
        component::EntityId eid = static_cast<uint8_t*>(obj->getUserPointer()) - static_cast<uint8_t*>(0);
        btScalar linearDamping = body->getLinearDamping();
        btScalar angularDamping = body->getAngularDamping();
        btVector3 linearVelocity = body->getLinearVelocity();
        btVector3 angularVelocity = body->getAngularVelocity();
        auto t = component::getComponent<component::Transform>(eid);
        auto rb = component::getComponent<component::RigidBody>(eid);
        t->position = {trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()};

        float yaw, pitch, roll;
        trans.getRotation().getEulerZYX(yaw, pitch, roll);
        t->orientation = quat({-roll, -pitch, -yaw});
    }
}

void BulletEngine::stop() {
    _running = false;

    // Delete rigid bodies
    for (int i = _world->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject* obj = _world->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
            delete body->getMotionState();
        _world->removeCollisionObject(obj);
        delete obj;
    }

    // Delete collision shapes
    for (size_t i = 0; i < _collisionShapes.size(); i++) {
        delete _collisionShapes[i];
        _collisionShapes[i] = nullptr;
    }
    _collisionShapes.clear();

    // Delete world
    _world.reset();
}

std::vector<component::EntityId> BulletEngine::getEntityCollisions(component::EntityId eid) { return {}; }

std::vector<component::EntityId> BulletEngine::rayCast(vec3 begin, vec3 end, bool onlyFirst) { return {}; }

bool BulletEngine::areColliding(component::EntityId eid0, component::EntityId eid1) { return false; }

void BulletEngine::createRigidBody(component::EntityId entity) {
    auto t = component::getComponent<component::Transform>(entity);
    auto rb = component::getComponent<component::RigidBody>(entity);
    auto box = component::getComponent<component::BoxCollider>(entity);
    auto sphere = component::getComponent<component::SphereCollider>(entity);

    if (!rb)
        return;

    if (!t && rb) {
        LOG_WARN("physics::BulletEngine", "Entity [w]$0[] is a rigid body but does not have a transform component", entity);
        return;
    }

    if (rb && !(box || sphere)) {
        LOG_WARN("physics::BulletEngine", "Entity [w]$0[] is a rigid body but does not have any collider component", entity);
        return;
    }

    if (box && sphere) {
        LOG_WARN("physics::BulletEngine", "Entity [w]$0[] must have only one collider", entity);
        return;
    }

    vec3 position, scale;
    quat orientation;
    mat4 m = t->getWorldTransform(entity);
    m.getPosOriScale(position, orientation, scale);

    // Calculate transform
    btTransform bodyTransform(btQuaternion(orientation.i, orientation.j, orientation.k, orientation.r),
                              btVector3(position.x, position.y, position.z));

    // Create collision shape
    btCollisionShape* colShape;
    if (box)
        colShape = new btBoxShape(btVector3(scale.x * box->size.x / 2.0f, scale.y * box->size.y / 2.0f, scale.z * box->size.z / 2.0f));
    else if (sphere)
        colShape = new btSphereShape(btScalar(std::max(std::max(scale.x, scale.y), scale.z) * sphere->radius));

    _collisionShapes.push_back(colShape);

    // Calculate mass and inertia
    btScalar mass = rb->mass;
    btVector3 localInertia(0, 0, 0);
    if (rb->type == component::RigidBody::DYNAMIC)
        colShape->calculateLocalInertia(mass, localInertia);
    else
        mass = 0.0f;

    // Create rigid body
    btDefaultMotionState* motionState = new btDefaultMotionState(bodyTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);
    body->setUserPointer(reinterpret_cast<uint8_t*>(entity));

    // add the body to the dynamics world
    _world->addRigidBody(body);
    //_bodyToEntity[body] = entity;
}

void BulletEngine::createPrismaticJoint(component::PrismaticJoint* prismatic) {}

void BulletEngine::createRevoluteJoint(component::RevoluteJoint* revolute) {}

void BulletEngine::createRigidJoint(component::RigidJoint* rigid) {}

} // namespace atta::physics
