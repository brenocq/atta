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
#include <atta/physics/engines/bulletEngine.h>
#include <atta/physics/interface.h>

#define BT_USRPTR_TO_EID(usrPtr) component::EntityId(static_cast<uint8_t*>(usrPtr) - static_cast<uint8_t*>(0))

namespace atta::physics {

BulletEngine::BulletEngine() : Engine(Engine::BULLET), _numSubSteps(1), _showAabb(false) {}

BulletEngine::~BulletEngine() {
    if (_running)
        stop();
}

void BulletEngine::start() {
    _running = true;
    gContactStartedCallback = collisionStarted;
    gContactEndedCallback = collisionEnded;

    _collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
    _dispatcher = std::make_shared<btCollisionDispatcher>(_collisionConfiguration.get());
    _broadPhase = std::make_shared<btDbvtBroadphase>();
    _solver = std::make_shared<btSequentialImpulseConstraintSolver>();

    _world = std::make_shared<btDiscreteDynamicsWorld>(_dispatcher.get(), _broadPhase.get(), _solver.get(), _collisionConfiguration.get());
    updateGravity();

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
    // Check bullet position and orientation consistent with component::Transform
    for (int j = _world->getNumCollisionObjects() - 1; j >= 0; j--) {
        btCollisionObject* obj = _world->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        component::EntityId eid = BT_USRPTR_TO_EID(obj->getUserPointer());

        btTransform trans;
        //if (body && body->getMotionState())
        //    body->getMotionState()->getWorldTransform(trans);
        //else
            trans = obj->getWorldTransform();

        // Check if object was translated
        auto t = component::getComponent<component::Transform>(eid);

        // Get atta world position and orientation
        vec3 position, scale;
        quat orientation;
        mat4 m = t->getWorldTransform(eid);
        m.getPosOriScale(position, orientation, scale);

        btQuaternion q = trans.getRotation();
        quat physicsOrientation(q.w(), -q.x(), -q.y(), -q.z());
        vec3 physicsPosition(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
        if (position != physicsPosition || orientation != physicsOrientation) {
            trans = btTransform(btQuaternion(-orientation.i, -orientation.j, -orientation.k, orientation.r), btVector3(position.x, position.y, position.z));
            body->setWorldTransform(trans);
            body->activate();
        }
    }

    // Step simulation
    _world->stepSimulation(dt, _numSubSteps, dt / _numSubSteps);

    // Update component::Transform with new position/orientation
    for (int j = _world->getNumCollisionObjects() - 1; j >= 0; j--) {
        btCollisionObject* obj = _world->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        //if (body && body->getMotionState())
        //    body->getMotionState()->getWorldTransform(trans);
        //else
            trans = obj->getWorldTransform();

        // Update transform
        component::EntityId eid = BT_USRPTR_TO_EID(obj->getUserPointer());
        auto t = component::getComponent<component::Transform>(eid);
        t->position = {trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()};
        btQuaternion q = trans.getRotation();
        t->orientation = quat(q.w(), -q.x(), -q.y(), -q.z());

        // Update rigid body
        //auto rb = component::getComponent<component::RigidBody>(eid);
        //btScalar linearDamping = body->getLinearDamping();
        //btScalar angularDamping = body->getAngularDamping();
        //btVector3 linearVelocity = body->getLinearVelocity();
        //btVector3 angularVelocity = body->getAngularVelocity();
    }
}

void BulletEngine::stop() {
    _running = false;
    _bodyToEntity.clear();
    _entityToBody.clear();
    _collisions.clear();
    gContactStartedCallback = nullptr;
    gContactEndedCallback = nullptr;

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

std::vector<component::EntityId> BulletEngine::getEntityCollisions(component::EntityId eid) {
    std::vector<component::EntityId> result;
    if (_collisions.find(eid) != _collisions.end())
        for (auto [other, data] : _collisions[eid])
            result.push_back(other);
    return result;
}

std::vector<component::EntityId> BulletEngine::rayCast(vec3 begin, vec3 end, bool onlyFirst) { return {}; }

bool BulletEngine::areColliding(component::EntityId eid0, component::EntityId eid1) {
    return _collisions.find(eid0) != _collisions.end() && _collisions[eid0].find(eid1) != _collisions[eid0].end();
}

void BulletEngine::updateGravity() {
    if (_running) {
        // Activate all non-static bodies
        btAlignedObjectArray<btRigidBody*>& rigidBodies = _world->getNonStaticRigidBodies();
        for (int i = 0; i < rigidBodies.size(); i++) {
            btRigidBody* body = rigidBodies[i];
            body->activate();
        }

        // Update gravity
        vec3 g = physics::getGravity();
        _world->setGravity(btVector3(g.x, g.y, g.z));
    }
}

unsigned BulletEngine::getNumSubSteps() const { return _numSubSteps; }
void BulletEngine::setNumSubSteps(unsigned numSubSteps) { _numSubSteps = numSubSteps; }
bool BulletEngine::getShowAabb() const { return _showAabb; }
void BulletEngine::setShowAabb(bool showAabb) { _showAabb = showAabb; }

btRigidBody* BulletEngine::getBulletRigidBody(component::EntityId entity) {
    if (!_running)
        return {};
    if (_entityToBody.find(entity) != _entityToBody.end())
        return _entityToBody[entity];
    return {};
}

bnd3 BulletEngine::getAabb(component::EntityId entity) {
    btRigidBody* rb = getBulletRigidBody(entity);
    if (rb) {
        btVector3 aabbMin, aabbMax;
        rb->getAabb(aabbMin, aabbMax);
        return bnd3(vec3(aabbMin.x(), aabbMin.y(), aabbMin.z()), vec3(aabbMax.x(), aabbMax.y(), aabbMax.z()));
    } else
        LOG_WARN("physics::BulletEngine", "Trying to get bullet aabb of entity [w]$0[], but this entity does not have a bullet rigid body", entity);

    return {};
}

const std::unordered_map<component::EntityId, std::unordered_map<component::EntityId, btPersistentManifold*>>& BulletEngine::getCollisions() {
    return _collisions;
}

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
    btTransform bodyTransform(btQuaternion(-orientation.i, -orientation.j, -orientation.k, orientation.r),
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
    _bodyToEntity[body] = entity;
    _entityToBody[entity] = body;
}

void BulletEngine::createPrismaticJoint(component::PrismaticJoint* prismatic) {}

void BulletEngine::createRevoluteJoint(component::RevoluteJoint* revolute) {}

void BulletEngine::createRigidJoint(component::RigidJoint* rigid) {}

void BulletEngine::collisionStarted(btPersistentManifold* const& manifold) {
    auto bullet = physics::getEngine<BulletEngine>();
    const btCollisionObject* obA = manifold->getBody0();
    const btCollisionObject* obB = manifold->getBody1();
    component::EntityId eidA = BT_USRPTR_TO_EID(obA->getUserPointer());
    component::EntityId eidB = BT_USRPTR_TO_EID(obB->getUserPointer());
    bullet->_collisions[eidA][eidB] = manifold;
    bullet->_collisions[eidB][eidA] = manifold;
}

void BulletEngine::collisionEnded(btPersistentManifold* const& manifold) {
    auto bullet = physics::getEngine<BulletEngine>();
    const btCollisionObject* obA = manifold->getBody0();
    const btCollisionObject* obB = manifold->getBody1();
    component::EntityId eidA = BT_USRPTR_TO_EID(obA->getUserPointer());
    component::EntityId eidB = BT_USRPTR_TO_EID(obB->getUserPointer());
    bullet->_collisions[eidA].erase(eidB);
    bullet->_collisions[eidB].erase(eidA);
}

} // namespace atta::physics
