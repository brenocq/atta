//--------------------------------------------------
// Atta Physics Module
// box2DEngine.cpp
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/boxCollider2D.h>
#include <atta/component/components/circleCollider2D.h>
#include <atta/component/components/polygonCollider2D.h>
#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>
#include <atta/physics/engines/box2DEngine.h>
#include <atta/physics/interface.h>
#include <atta/utils/config.h>

namespace atta::physics {

//---------- Callbacks ----------//
class ContactListener : public b2ContactListener {
  public:
    ContactListener(std::unordered_map<component::EntityId, std::unordered_set<component::EntityId>>& collisions) : _collisions(collisions) {}

    void BeginContact(b2Contact* contact) override {
        if (contact->IsTouching()) {
            component::EntityId eidA = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
            component::EntityId eidB = contact->GetFixtureB()->GetBody()->GetUserData().pointer;

            _collisions[eidA].insert(eidB);
            _collisions[eidB].insert(eidA);
        }
    }

    void EndContact(b2Contact* contact) override {
        component::EntityId eidA = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        component::EntityId eidB = contact->GetFixtureB()->GetBody()->GetUserData().pointer;

        _collisions[eidA].erase(eidB);
        _collisions[eidB].erase(eidA);
    }

  private:
    std::unordered_map<component::EntityId, std::unordered_set<component::EntityId>>& _collisions;
};

class EntityCollisionQueryCallback : public b2QueryCallback {
  public:
    bool ReportFixture(b2Fixture* fixture) override {
        entities.push_back(fixture->GetBody()->GetUserData().pointer);
        return true;
    }

    std::vector<component::EntityId> entities;
};

class RayCastCallback : public b2RayCastCallback {
  public:
    RayCastCallback(bool onlyFirst, vec2 start) : _onlyFirst(onlyFirst), _start(start) {}

    float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
        if (_onlyFirst)
            hits.clear();
        RayCastHit hit{};
        hit.entity = fixture->GetBody()->GetUserData().pointer;
        hit.distance = length(vec2(point.x, point.y) - _start);
        hit.normal = vec3(normal.x, normal.y, 0.0f);
        hits.push_back(hit);
        return _onlyFirst ? fraction : 1;
    }

    std::vector<RayCastHit> hits;

  private:
    bool _onlyFirst;
    vec2 _start;
};

//---------- Conversions ----------//
inline b2BodyType attaToBox2D(component::RigidBody2D::Type type) {
    switch (type) {
        case component::RigidBody2D::DYNAMIC:
            return b2_dynamicBody;
        case component::RigidBody2D::KINEMATIC:
            return b2_kinematicBody;
        case component::RigidBody2D::STATIC:
            return b2_staticBody;
    }
    return b2_staticBody;
}

//---------- Box2DEngine ----------//
Box2DEngine::Box2DEngine() : Engine(Engine::BOX2D) {}

Box2DEngine::~Box2DEngine() {
    if (_running)
        stop();
}

void Box2DEngine::start() {
    _running = true;
    vec2 g = vec2(physics::getGravity());
    _world = std::make_shared<b2World>(b2Vec2(g.x, g.y));

    // Create contact listener
    // TODO need to free?
    ContactListener* contactlistener = new ContactListener(_collisions);
    _world->SetContactListener(contactlistener);

    //---------- Create ground body----------//
    // This body is used to apply top-down friction if necessary
    b2BodyDef groundBodyDef{};
    _groundBody = _world->CreateBody(&groundBodyDef);

    //---------- Create rigid bodies ----------//
    std::vector<component::EntityId> entities = component::getNoPrototypeView();
    for (component::EntityId entity : entities) {
        auto t = component::getComponent<component::Transform>(entity);
        auto rb2d = component::getComponent<component::RigidBody2D>(entity);
        auto box2d = component::getComponent<component::BoxCollider2D>(entity);
        auto circle2d = component::getComponent<component::CircleCollider2D>(entity);
        auto polygon2d = component::getComponent<component::PolygonCollider2D>(entity);

        if (!rb2d)
            continue;

        if (!t && rb2d) {
            LOG_WARN("physics::box2DEngine", "Entity [w]$0[] is a rigid body but does not have a transform component", entity);
            continue;
        }

        int numColliders = ((box2d != nullptr) + (circle2d != nullptr) + (polygon2d != nullptr));

        if (numColliders == 0) {
            LOG_WARN("physics::box2DEngine", "Entity [w]$0[] is a rigid body but does not have any collider component", entity);
            continue;
        }

        if (numColliders > 1) {
            LOG_WARN("physics::box2DEngine", "Entity [w]$0[] must have only one collider", entity);
            continue;
        }

        createRigidBody(entity);
        createColliders(entity);
    }

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

void Box2DEngine::step(float dt) {
    int velocityIterations = 8;
    int positionIterations = 3;

    //----- Update box2d bodies -----//
    for (auto [eid, body] : _bodies) {
        auto t = component::getComponent<component::Transform>(eid);
        auto rb2d = component::getComponent<component::RigidBody2D>(eid);

        // Check type change
        if (body->GetType() != attaToBox2D(rb2d->type))
            body->SetType(attaToBox2D(rb2d->type));

        // Get atta pos/angle
        component::Transform trans = t->getWorldTransform(eid);

        // Get box2d pos/angle
        b2Vec2 pos = body->GetPosition();
        vec3 physicsPosition = {pos.x, pos.y, trans.position.z};
        quat physicsOrientation;
        physicsOrientation.set2DAngle(body->GetAngle());

        // Calculate quaternion distance
        quat po = physicsOrientation;
        quat o = trans.orientation;
        float qDist = po.r * o.r + po.i * o.i + po.j * o.j + po.k * o.k;
        qDist = 1 - qDist * qDist; // 0 if they are the same, 1 if they are opposite
        bool isSameOri = qDist < 0.001 || qDist > 0.999;

        // Check if need to update physics transform
        if (trans.position != physicsPosition || !isSameOri) {
            body->SetTransform(b2Vec2(trans.position.x, trans.position.y), trans.orientation.get2DAngle());
            body->SetAwake(true);
        }
    }

    //----- Step simulation -----//
    _world->Step(dt, velocityIterations, positionIterations);

    //----- Update atta components -----//
    for (auto [eid, body] : _bodies) {
        auto t = component::getComponent<component::Transform>(eid);
        if (body->IsAwake() && t) {
            component::Transform worldTrans;

            // Get new transform (after physics step)
            b2Vec2 pos = body->GetPosition();
            worldTrans.position = {pos.x, pos.y, t->position.z};
            worldTrans.orientation.set2DAngle(body->GetAngle());
            worldTrans.scale = t->scale;

            // Update transform
            t->setWorldTransform(eid, worldTrans);
        }
    }
}

void Box2DEngine::stop() {
    _running = false;
    _bodies.clear();
    _componentToEntity.clear();
    _collisions.clear();
    _world.reset();
}

void Box2DEngine::createRigidBody(component::EntityId entity) {
    if (_bodies.find(entity) != _bodies.end())
        deleteRigidBody(entity);

    auto t = component::getComponent<component::Transform>(entity);
    auto rb2d = component::getComponent<component::RigidBody2D>(entity);

    // Get world transform
    component::Transform worldT = t->getWorldTransform(entity);
    vec3 position = worldT.position;
    quat orientation = worldT.orientation;

    // Create box2d body definition
    b2BodyDef bodyDef;
    bodyDef.type = attaToBox2D(rb2d->type);
    bodyDef.position.Set(position.x, position.y);
    bodyDef.angle = orientation.get2DAngle();
    bodyDef.linearVelocity.Set(rb2d->linearVelocity.x, rb2d->linearVelocity.y);
    bodyDef.angularVelocity = rb2d->angularVelocity;
    bodyDef.linearDamping = rb2d->linearDamping;
    bodyDef.angularDamping = rb2d->angularDamping;
    bodyDef.allowSleep = rb2d->allowSleep;
    bodyDef.awake = rb2d->awake;
    bodyDef.fixedRotation = rb2d->fixedRotation;
    bodyDef.userData.pointer = entity;

    // Create body
    b2Body* body = _world->CreateBody(&bodyDef);
    _bodies[entity] = body;

    // Apply top-down friction
    vec3 gravity = physics::getGravity();
    if (gravity.z && rb2d->groundFriction) {
        b2FrictionJointDef frictionJointDef;
        frictionJointDef.bodyA = _groundBody;
        frictionJointDef.bodyB = body;
        frictionJointDef.maxForce = (-gravity.z * rb2d->mass) * rb2d->friction;               // Set the maximum friction force
        frictionJointDef.maxTorque = frictionJointDef.maxForce * vec2(t->scale).length() / 2; // Set the maximum friction torque
        _world->CreateJoint(&frictionJointDef);
    }

    // Register component pointer to EntityId conversion
    _componentToEntity[rb2d] = entity;
}

void Box2DEngine::deleteRigidBody(component::EntityId entity) {
    _world->DestroyBody(_bodies[entity]);
    _bodies.erase(entity);
}

void Box2DEngine::createColliders(component::EntityId entity) {
    auto t = component::getComponent<component::Transform>(entity);
    auto rb2d = component::getComponent<component::RigidBody2D>(entity);
    auto box2d = component::getComponent<component::BoxCollider2D>(entity);
    auto circle2d = component::getComponent<component::CircleCollider2D>(entity);
    auto polygon2d = component::getComponent<component::PolygonCollider2D>(entity);

    // Get world transform
    component::Transform worldT = t->getWorldTransform(entity);
    vec3 scale = worldT.scale;
    b2FixtureDef fixtureDef;

    // Shapes
    b2PolygonShape polygonShape;
    b2CircleShape circle;

    // Create shape
    float area = 1.0f;
    if (box2d) {
        polygonShape.SetAsBox(scale.x * box2d->size.x / 2.0f, scale.y * box2d->size.y / 2.0f);
        fixtureDef.shape = &polygonShape;
        // Compute area
        area = scale.x * box2d->size.x * scale.y * box2d->size.y;
    } else if (circle2d) {
        circle.m_radius = std::max(scale.x, scale.y) * circle2d->radius;
        fixtureDef.shape = &circle;
        // Compute area
        area = circle.m_radius * circle.m_radius * 3.14159265f;
    } else if (polygon2d) {
        // Set polygon points
        std::vector<b2Vec2> b2Points;
        for (const auto& point : polygon2d->points)
            b2Points.emplace_back(scale.x * point.x, scale.y * point.y);
        polygonShape.Set(b2Points.data(), static_cast<int32>(b2Points.size()));
        // Compute area
        b2MassData massData;
        polygonShape.ComputeMass(&massData, 1.0f);
        area = massData.mass;
        fixtureDef.shape = &polygonShape;
    }

    if (area == 0.0f) {
        LOG_WARN("Box2DEngine", "Rigid body collision shape should not have zero area");
        area = 1.0f;
    }

    // Material properties
    fixtureDef.density = rb2d->mass / area;
    fixtureDef.friction = rb2d->friction;
    fixtureDef.restitution = rb2d->restitution;
    fixtureDef.restitutionThreshold = 0.1f;

    // Attach collider
    _bodies[entity]->CreateFixture(&fixtureDef);
}

void Box2DEngine::deleteColliders(component::EntityId entity) {
    if (_bodies.find(entity) != _bodies.end()) {
        b2Fixture* fixture = _bodies[entity]->GetFixtureList();
        if (fixture)
            _bodies[entity]->DestroyFixture(fixture);
    }
}

void Box2DEngine::createPrismaticJoint(component::PrismaticJoint* prismatic) {
    b2Body* bodyA;
    b2Body* bodyB;
    if (_bodies.find(prismatic->bodyA) == _bodies.end() || _bodies.find(prismatic->bodyB) == _bodies.end()) {
        LOG_WARN("Box2DEngine", "Trying to create joint with entity that is not a rigid body");
        return;
    } else {
        bodyA = _bodies[prismatic->bodyA];
        bodyB = _bodies[prismatic->bodyB];
    }

    auto t = component::getComponent<component::Transform>(prismatic->bodyA);

    b2PrismaticJointDef pjd;
    pjd.Initialize(bodyA, bodyB, b2Vec2(t->position.x, t->position.y), b2Vec2(prismatic->axisA.x, prismatic->axisA.y));

    pjd.localAnchorA = b2Vec2(prismatic->anchorA.x, prismatic->anchorA.y);
    pjd.localAnchorB = b2Vec2(prismatic->anchorB.x, prismatic->anchorB.y);

    pjd.enableLimit = prismatic->enableLimits;
    pjd.lowerTranslation = prismatic->lowerLimit;
    pjd.upperTranslation = prismatic->upperLimit;

    pjd.enableMotor = prismatic->enableMotor;
    pjd.motorSpeed = prismatic->targetMotorVelocity;
    pjd.maxMotorForce = prismatic->maxMotorForce;

    _world->CreateJoint(&pjd);
}

void Box2DEngine::createRevoluteJoint(component::RevoluteJoint* revolute) {
    b2Body* bodyA;
    b2Body* bodyB;
    if (_bodies.find(revolute->bodyA) == _bodies.end() || _bodies.find(revolute->bodyB) == _bodies.end()) {
        LOG_WARN("Box2DEngine", "Trying to create joint with entity that is not a rigid body");
        return;
    } else {
        bodyA = _bodies[revolute->bodyA];
        bodyB = _bodies[revolute->bodyB];
    }

    auto t = component::getComponent<component::Transform>(revolute->bodyA);

    b2RevoluteJointDef rjd;
    rjd.Initialize(bodyA, bodyB, b2Vec2(t->position.x, t->position.y));

    rjd.localAnchorA = b2Vec2(revolute->anchorA.x, revolute->anchorA.y);
    rjd.localAnchorB = b2Vec2(revolute->anchorB.x, revolute->anchorB.y);

    rjd.enableLimit = revolute->enableLimits;
    rjd.lowerAngle = revolute->lowerAngle;
    rjd.upperAngle = revolute->upperAngle;

    rjd.enableMotor = revolute->enableMotor;
    rjd.motorSpeed = revolute->targetMotorVelocity;
    rjd.maxMotorTorque = revolute->maxMotorForce;

    _world->CreateJoint(&rjd);
}

void Box2DEngine::createRigidJoint(component::RigidJoint* rigid) {
    b2Body* bodyA;
    b2Body* bodyB;
    if (_bodies.find(rigid->bodyA) == _bodies.end() || _bodies.find(rigid->bodyB) == _bodies.end()) {
        LOG_WARN("Box2DEngine", "Trying to create joint with entity that is not a rigid body");
        return;
    } else {
        bodyA = _bodies[rigid->bodyA];
        bodyB = _bodies[rigid->bodyB];
    }

    auto ta = component::getComponent<component::Transform>(rigid->bodyA);
    auto tb = component::getComponent<component::Transform>(rigid->bodyB);
    vec3 worldDir = tb->position - ta->position;
    vec3 localDir = inverse(ta->getWorldTransformMatrix(rigid->bodyA)) * worldDir;
    vec3 localDirNorm = normalize(localDir);
    b2Vec2 axis = b2Vec2(localDirNorm.x, localDirNorm.y);
    b2Vec2 anchor = b2Vec2(localDir.x, localDir.y);

    b2PrismaticJointDef pjd;
    pjd.Initialize(bodyA, bodyB, anchor, axis);
    pjd.enableLimit = true;
    pjd.lowerTranslation = localDir.length();
    pjd.upperTranslation = localDir.length();
    pjd.enableMotor = false;

    _world->CreateJoint(&pjd);
}

std::vector<component::EntityId> Box2DEngine::getEntityCollisions(component::EntityId eid) {
    if (_collisions.find(eid) != _collisions.end()) {
        std::vector<component::EntityId> collisions(_collisions[eid].begin(), _collisions[eid].end());
        return collisions;
    } else
        return {};
}

std::vector<RayCastHit> Box2DEngine::rayCast(vec3 begin, vec3 end, bool onlyFirst) {
    RayCastCallback rc(onlyFirst, vec2(begin));
    _world->RayCast(&rc, b2Vec2(begin.x, begin.y), b2Vec2(end.x, end.y));

    return rc.hits;
}

bool Box2DEngine::areColliding(component::EntityId eid0, component::EntityId eid1) {
    return _collisions.find(eid0) != _collisions.end() && _collisions[eid0].find(eid1) != _collisions[eid0].end();
}

void Box2DEngine::updateGravity() {
    if (Config::getState() != Config::State::IDLE) {
        vec3 g = physics::getGravity();
        _world->SetGravity(b2Vec2(g.x, g.y));
    }
}

b2Body* Box2DEngine::getBox2DRigidBody(component::EntityId entity) { return _bodies.find(entity) != _bodies.end() ? _bodies[entity] : nullptr; }

std::vector<component::EntityId> Box2DEngine::getAABBEntities(vec2 lower, vec2 upper) {
    EntityCollisionQueryCallback callback;

    b2AABB aabb;
    aabb.lowerBound.Set(lower.x, lower.y);
    aabb.upperBound.Set(upper.x, upper.y);
    _world->QueryAABB(&callback, aabb);

    return callback.entities;
}

// component::RigidBody2D interface
void Box2DEngine::setTransform(component::RigidBody2D* rb2d, vec2 position, float angle) {
    if (Config::getState() != Config::State::IDLE)
        _bodies[_componentToEntity[rb2d]]->SetTransform(b2Vec2(position.x, position.y), angle);
}

void Box2DEngine::setLinearVelocity(component::RigidBody2D* rb2d, vec2 vel) {
    if (Config::getState() != Config::State::IDLE)
        _bodies[_componentToEntity[rb2d]]->SetLinearVelocity(b2Vec2(vel.x, vel.y));
}

void Box2DEngine::setAngularVelocity(component::RigidBody2D* rb2d, float omega) {
    if (Config::getState() != Config::State::IDLE)
        _bodies[_componentToEntity[rb2d]]->SetAngularVelocity(omega);
}

void Box2DEngine::applyForce(component::RigidBody2D* rb2d, vec2 force, vec2 point, bool wake) {
    if (Config::getState() != Config::State::IDLE)
        _bodies[_componentToEntity[rb2d]]->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), wake);
}

void Box2DEngine::applyForceToCenter(component::RigidBody2D* rb2d, vec2 force, bool wake) {
    if (Config::getState() != Config::State::IDLE)
        _bodies[_componentToEntity[rb2d]]->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
}

void Box2DEngine::applyTorque(component::RigidBody2D* rb2d, float torque, bool wake) {
    if (Config::getState() != Config::State::IDLE)
        _bodies[_componentToEntity[rb2d]]->ApplyTorque(torque, wake);
}

} // namespace atta::physics
