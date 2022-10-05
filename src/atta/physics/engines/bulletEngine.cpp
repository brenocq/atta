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
#include <atta/utils/config.h>

#define BT_USRPTR_TO_EID(usrPtr) component::EntityId(static_cast<uint8_t*>(usrPtr) - static_cast<uint8_t*>(0))

namespace atta::physics {

constexpr int USER_TYPE_PRISMATIC = 0;
constexpr int USER_TYPE_REVOLUTE = 1;

BulletEngine::BulletEngine() : Engine(Engine::BULLET), _numSubSteps(1), _showAabb(false) {}

BulletEngine::~BulletEngine() {
    if (_running)
        stop();
}

//----------------------------------------------//
//------------------- START --------------------//
//----------------------------------------------//
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

//----------------------------------------------//
//-------------------- STEP --------------------//
//----------------------------------------------//
void BulletEngine::step(float dt) {
    //----- Update bullet rigid bodies -----//
    for (int j = _world->getNumCollisionObjects() - 1; j >= 0; j--) {
        btCollisionObject* obj = _world->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        component::EntityId eid = BT_USRPTR_TO_EID(obj->getUserPointer());

        // Update mass
        auto rb = component::getComponent<component::RigidBody>(eid);
        if (rb->type == component::RigidBody::DYNAMIC)
            body->setMassProps(rb->mass, body->getLocalInertia());

        // Get atta transform
        auto t = component::getComponent<component::Transform>(eid);
        vec3 position, scale;
        quat orientation;
        mat4 m = t->getWorldTransform(eid);
        m.getPosOriScale(position, orientation, scale);

        // Get bullet transform
        btTransform trans;
        trans = obj->getWorldTransform();
        quat physicsOrientation(trans.getRotation().w(), -trans.getRotation().x(), -trans.getRotation().y(), -trans.getRotation().z());
        vec3 physicsPosition(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

        // Check if atta and bullet transforms match
        if (position != physicsPosition || orientation != physicsOrientation) {
            trans = btTransform(btQuaternion(-orientation.i, -orientation.j, -orientation.k, orientation.r),
                                btVector3(position.x, position.y, position.z));
            body->setWorldTransform(trans);

            // Activate this body
            body->activate();

            // Activate bodies linked to this body
            if (_connectedEntities.find(eid) != _connectedEntities.end())
                for (auto other : _connectedEntities[eid])
                    if (_entityToBody.find(other) != _entityToBody.end())
                        _entityToBody[other]->activate();
        }
    }

    //----- Update bullet constraints -----//
    for (int i = 0; i < _world->getNumConstraints(); i++) {
        btTypedConstraint* c = _world->getConstraint(i);
        switch (c->getUserConstraintType()) {
            case USER_TYPE_PRISMATIC: {
                component::PrismaticJoint* prismatic = (component::PrismaticJoint*)c->getUserConstraintPtr();
                btSliderConstraint* btSlider = (btSliderConstraint*)(c);
                btSlider->setMaxLinMotorForce(prismatic->maxMotorForce);
                btSlider->setTargetLinMotorVelocity(prismatic->targetMotorVelocity);
                break;
            }
            case USER_TYPE_REVOLUTE: {
                component::RevoluteJoint* revolute = (component::RevoluteJoint*)c->getUserConstraintPtr();
                btHingeConstraint* btHinge = (btHingeConstraint*)(c);
                btHinge->setMaxMotorImpulse(revolute->maxMotorForce * Config::getDt());
                btHinge->setMotorTargetVelocity(revolute->targetMotorVelocity);
                //btHinge->enableAngularMotor(true, revolute->targetMotorVelocity, revolute->maxMotorForce * Config::getDt());
                break;
            }
        }
    }

    //----- Step simulation -----//
    _world->stepSimulation(dt, _numSubSteps, dt / _numSubSteps);

    //----- Update atta rigid body -----//
    for (int j = _world->getNumCollisionObjects() - 1; j >= 0; j--) {
        // TODO Should go through collisionObjects from the root down the relationship hierarchy,
        // strange simulations may happen otherwise because of wrong parent transform
        btCollisionObject* obj = _world->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        trans = obj->getWorldTransform();

        // Calculate world transform
        component::EntityId eid = BT_USRPTR_TO_EID(obj->getUserPointer());
        auto t = component::getComponent<component::Transform>(eid);
        vec3 pos = {trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()};
        quat ori = quat(trans.getRotation().w(), -trans.getRotation().x(), -trans.getRotation().y(), -trans.getRotation().z());
        mat4 worldTransform;
        worldTransform.setPosOriScale(pos, ori, t->scale);

        // Update world transform
        t->setWorldTransform(eid, worldTransform);
        mat4 m = t->getWorldTransform(eid);

        // Update rigid body
        // auto rb = component::getComponent<component::RigidBody>(eid);
        // btScalar linearDamping = body->getLinearDamping();
        // btScalar angularDamping = body->getAngularDamping();
        // btVector3 linearVelocity = body->getLinearVelocity();
        // btVector3 angularVelocity = body->getAngularVelocity();
    }

    //----- Update atta joints -----//
    for (int i = 0; i < _world->getNumConstraints(); i++) {
        btTypedConstraint* c = _world->getConstraint(i);
        switch (c->getUserConstraintType()) {
            case USER_TYPE_PRISMATIC: {
                component::PrismaticJoint* prismatic = (component::PrismaticJoint*)c->getUserConstraintPtr();
                btSliderConstraint* btSlider = (btSliderConstraint*)(c);

                // Update position
                prismatic->motorPosition = btSlider->getLinearPos();
                break;
            }
            case USER_TYPE_REVOLUTE: {
                component::RevoluteJoint* revolute = (component::RevoluteJoint*)c->getUserConstraintPtr();
                btHingeConstraint* btHinge = (btHingeConstraint*)(c);

                // Update position
                revolute->motorAngle = btHinge->getHingeAngle();
                break;
            }
        }
    }
}

//----------------------------------------------//
//-------------------- STOP --------------------//
//----------------------------------------------//
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

//----------------------------------------------//
//----------------- RIGID BODY -----------------//
//----------------------------------------------//
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

//----------------------------------------------//
//----------------- PRISMATIC ------------------//
//----------------------------------------------//
void BulletEngine::createPrismaticJoint(component::PrismaticJoint* prismatic) {
    btRigidBody* rbA = getBulletRigidBody(prismatic->bodyA);
    btRigidBody* rbB = getBulletRigidBody(prismatic->bodyB);
    if (!rbA || !rbB) {
        LOG_WARN("physics::BulletEngine", "Could not create prismatic joint between [w]$0[] and [w]$1[]", prismatic->bodyA, prismatic->bodyB);
        return;
    }

    // Calculate orientation and axis
    quat qA;
    qA.setRotationFromVectors(prismatic->axisA, vec3(1, 0, 0));
    quat qB;
    qB.setRotationFromVectors(-prismatic->axisB, vec3(1, 0, 0));
    vec3 aA = prismatic->anchorA;
    vec3 aB = prismatic->anchorB;

    // Calculate frames
    btTransform frameInA(btQuaternion(-qA.i, -qA.j, -qA.k, qA.r), btVector3(aA.x, aA.y, aA.z));
    btTransform frameInB(btQuaternion(-qB.i, -qB.j, -qB.k, qB.r), btVector3(aB.x, aB.y, aB.z));

    // Create constraint
    btSliderConstraint* slider = new btSliderConstraint(*rbA, *rbB, frameInA, frameInB, true);
    slider->setUserConstraintType(USER_TYPE_PRISMATIC);
    slider->setUserConstraintPtr(prismatic);

    // Enable limits
    if (prismatic->enableLimits) {
        slider->setLowerLinLimit(prismatic->lowerLimit);
        slider->setUpperLinLimit(prismatic->upperLimit);
    }

    // Enable motor
    if (prismatic->enableMotor) {
        // Setup motor
        slider->setPoweredLinMotor(true);
        slider->setMaxLinMotorForce(prismatic->maxMotorForce);
        slider->setTargetLinMotorVelocity(prismatic->targetMotorVelocity);
        // Disable sleeping
        rbA->setSleepingThresholds(0.0f, 0.0f);
        rbB->setSleepingThresholds(0.0f, 0.0f);
    }

    // Should collision
    rbA->setIgnoreCollisionCheck(rbB, !prismatic->shouldCollide);

    // Add constraint
    _world->addConstraint(slider);

    // Keep track of connected entities
    _connectedEntities[prismatic->bodyA].push_back(prismatic->bodyB);
    _connectedEntities[prismatic->bodyB].push_back(prismatic->bodyA);
}

//----------------------------------------------//
//------------------ REVOLUTE ------------------//
//----------------------------------------------//
void BulletEngine::createRevoluteJoint(component::RevoluteJoint* revolute) {
    btRigidBody* rbA = getBulletRigidBody(revolute->bodyA);
    btRigidBody* rbB = getBulletRigidBody(revolute->bodyB);
    if (!rbA || !rbB) {
        LOG_WARN("physics::BulletEngine", "Could not create revolute joint between [w]$0[] and [w]$1[]", revolute->bodyA, revolute->bodyB);
        return;
    }

    // Create constraint
    btVector3 pivotInA{revolute->anchorA.x, revolute->anchorA.y, revolute->anchorA.z};
    btVector3 pivotInB{revolute->anchorB.x, revolute->anchorB.y, revolute->anchorB.z};
    btVector3 axisInA{revolute->axisA.x, revolute->axisA.y, revolute->axisA.z};
    btVector3 axisInB{revolute->axisB.x, revolute->axisB.y, revolute->axisB.z};

    btHingeConstraint* hinge = new btHingeConstraint(*rbA, *rbB, pivotInA, pivotInB, axisInA, axisInB, true);
    hinge->setUserConstraintType(USER_TYPE_REVOLUTE);
    hinge->setUserConstraintPtr(revolute);

    // Enable limits
    if (revolute->enableLimits)
        hinge->setLimit(revolute->lowerAngle, revolute->upperAngle);

    // Enable motor
    if (revolute->enableMotor) {
        // Setup motor
        //hinge->enableMotor(true);
        //hinge->setMaxMotorImpulse(revolute->maxMotorForce*Config::getDt());
        //hinge->setMotorTargetVelocity(revolute->targetMotorVelocity);
        hinge->enableAngularMotor(true, revolute->targetMotorVelocity, revolute->maxMotorForce * Config::getDt());
        // Disable sleeping
        rbA->setSleepingThresholds(0.0f, 0.0f);
        rbB->setSleepingThresholds(0.0f, 0.0f);
    }

    // Should collision
    rbA->setIgnoreCollisionCheck(rbB, !revolute->shouldCollide);

    // Add constraint
    _world->addConstraint(hinge, true);

    // Keep track of connected entities
    _connectedEntities[revolute->bodyA].push_back(revolute->bodyB);
    _connectedEntities[revolute->bodyB].push_back(revolute->bodyA);
}

//----------------------------------------------//
//------------------- RIGID --------------------//
//----------------------------------------------//
void BulletEngine::createRigidJoint(component::RigidJoint* rigid) {
    // Create CompoundShape from entities
}

//----------------------------------------------//
//----------------- COLLLISION -----------------//
//----------------------------------------------//
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
