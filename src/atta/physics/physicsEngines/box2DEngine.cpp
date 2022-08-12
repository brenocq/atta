//--------------------------------------------------
// Atta Physics Module
// box2DEngine.cpp
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/boxCollider.h>
#include <atta/component/components/relationship.h>
#include <atta/component/components/sphereCollider.h>
#include <atta/component/components/transform.h>
#include <atta/physics/physicsEngines/box2DEngine.h>

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
    RayCastCallback(bool onlyFirst) : _onlyFirst(onlyFirst) {}

    float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
        entities.push_back(fixture->GetBody()->GetUserData().pointer);
        return _onlyFirst ? 0 : 1;
    }

    std::vector<component::EntityId> entities;

  private:
    bool _onlyFirst;
};

//---------- Box2DEngine ----------//
Box2DEngine::Box2DEngine() : PhysicsEngine(PhysicsEngine::BOX2D_ENGINE) {}

Box2DEngine::~Box2DEngine() {
    if (_running)
        stop();
}

void Box2DEngine::start() {
    _running = true;
    _world = std::make_shared<b2World>(b2Vec2(0.0f, 0.0f));

    // Create contact listener
    // TODO need to free?
    ContactListener* contactlistener = new ContactListener(_collisions);
    _world->SetContactListener(contactlistener);

    std::vector<component::EntityId> entities = component::getNoPrototypeView();
    //---------- Create rigid bodies ----------//
    for (component::EntityId entity : entities) {
        auto t = component::getEntityComponent<component::Transform>(entity);
        auto rb2d = component::getEntityComponent<component::RigidBody2D>(entity);
        auto box2d = component::getEntityComponent<component::BoxCollider>(entity);
        auto circle2d = component::getEntityComponent<component::SphereCollider>(entity);

        if (!rb2d)
            continue;

        if (!t && rb2d) {
            LOG_WARN("physics::box2DEngine", "Entity [w]$0[] is a rigid body but does not have a transform component", entity);
            continue;
        }

        if (rb2d && !(box2d || circle2d)) {
            LOG_WARN("physics::box2DEngine", "Entity [w]$0[] is a rigid body but does not have any collider component", entity);
            continue;
        }

        if (box2d && circle2d) {
            LOG_WARN("physics::box2DEngine", "Entity [w]$0[] must have only one collider", entity);
            continue;
        }

        vec3 position, scale;
        quat orientation;
        mat4 m = t->getWorldTransform(entity);
        m.getPosOriScale(position, orientation, scale);

        if (t && rb2d && (box2d || circle2d)) {
            _componentToEntity[rb2d] = entity;

            //----- Create body -----//
            b2BodyDef bodyDef;
            if (rb2d->type == component::RigidBody2D::DYNAMIC)
                bodyDef.type = b2_dynamicBody;
            else if (rb2d->type == component::RigidBody2D::KINEMATIC)
                bodyDef.type = b2_kinematicBody;
            else if (rb2d->type == component::RigidBody2D::STATIC)
                bodyDef.type = b2_staticBody;

            bodyDef.position.Set(position.x, position.y);
            bodyDef.angle = -orientation.toEuler().z;
            bodyDef.linearVelocity.Set(rb2d->linearVelocity.x, rb2d->linearVelocity.y);
            bodyDef.angularVelocity = rb2d->angularVelocity;
            bodyDef.linearDamping = rb2d->linearDamping;
            bodyDef.angularDamping = rb2d->angularDamping;
            bodyDef.allowSleep = rb2d->allowSleep;
            bodyDef.awake = rb2d->awake;
            bodyDef.fixedRotation = rb2d->fixedRotation;
            bodyDef.userData.pointer = entity;

            b2Body* body = _world->CreateBody(&bodyDef);
            _bodies[entity] = body;

            //----- Attach collider -----//
            b2FixtureDef fixtureDef;
            // Shapes
            b2PolygonShape polygonShape;
            b2CircleShape circle;

            // Create shape
            float area = 1.0f;
            if (box2d) {
                area = scale.x * box2d->size.x * scale.y * box2d->size.y;
                polygonShape.SetAsBox(scale.x * box2d->size.x / 2.0f, scale.y * box2d->size.y / 2.0f);
                fixtureDef.shape = &polygonShape;
            } else if (circle2d) {
                circle.m_radius = std::max(scale.x, scale.y) * circle2d->radius;
                area = circle.m_radius * circle.m_radius * 3.14159265f;
                fixtureDef.shape = &circle;
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
            body->CreateFixture(&fixtureDef);
        }
    }

    //---------- Create joints ----------//
    for (component::EntityId entity : entities) {
        auto prismatic = component::getEntityComponent<component::PrismaticJoint>(entity);
        auto revolute = component::getEntityComponent<component::RevoluteJoint>(entity);
        auto rigid = component::getEntityComponent<component::RigidJoint>(entity);

        if (prismatic)
            createPrismaticJoint(prismatic);
        if (revolute)
            createRevoluteJoint(revolute);
        if (rigid)
            createRigidJoint(rigid);
    }
}

void Box2DEngine::step(float dt) {
    int velocityIterations = 6;
    int positionIterations = 2;
    _world->Step(dt, velocityIterations, positionIterations);

    // Update transform components
    for (auto p : _bodies) {
        auto t = component::getEntityComponent<component::Transform>(p.first);
        if (t) {
            // component::Transform data
            vec3 position;
            quat orientation;
            vec3 scale;

            // Get new transform (after physics step)
            b2Vec2 pos = p.second->GetPosition();
            position = {pos.x, pos.y, 0.0f};
            orientation.fromEuler({0, 0, -p.second->GetAngle()});
            scale = t->scale;

            // Update pos/ori/scale to be local to parent
            component::Relationship* r = component::getEntityComponent<component::Relationship>(p.first);
            if (r && r->getParent() != -1) {
                // Get transform of the first entity that has transform when going up in the hierarchy
                component::Transform* pt = nullptr;
                component::EntityId parentId = -1;
                while (pt == nullptr) {
                    parentId = r->getParent();
                    pt = component::getEntityComponent<component::Transform>(parentId);
                    r = component::getEntityComponent<component::Relationship>(parentId);
                    if (r->getParent() == -1)
                        break;
                }

                // If found some entity with transform component, convert result to be relative to it
                if (pt) {
                    vec3 pPos, pScale;
                    quat pOri;
                    mat4 pTransform = pt->getWorldTransform(parentId);
                    pTransform.getPosOriScale(pPos, pOri, pScale);

                    // Calculate pos ori scale relative to parent
                    position -= pPos;
                    scale /= pScale;
                    orientation = orientation * (-pOri); // Rotation from pOri to ori
                }
            }

            // Update transform
            t->position = position;
            t->orientation = orientation;
            t->scale = scale;
        }
    }

    // Testing
    // std::vector<component::EntityId> collisions = getEntityCollisions(p.first);
    // std::vector<component::EntityId> collisions = getRayCastCollisions({-10,-10,0}, {10,10,0}, false);
    // if(collisions.size())
    //    LOG_DEBUG("Box2DEngine", "Ray cast collisions: $0", collisions);
}

void Box2DEngine::stop() {
    _running = false;
    _bodies.clear();
    _componentToEntity.clear();
    _world.reset();
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

    auto t = component::getEntityComponent<component::Transform>(prismatic->bodyA);

    b2PrismaticJointDef pjd;
    pjd.Initialize(bodyA, bodyB, b2Vec2(t->position.x, t->position.y), b2Vec2(prismatic->axis.x, prismatic->axis.y));

    pjd.localAnchorA = b2Vec2(prismatic->anchorA.x, prismatic->anchorA.y);
    pjd.localAnchorB = b2Vec2(prismatic->anchorB.x, prismatic->anchorB.y);

    pjd.enableLimit = prismatic->enableLimits;
    pjd.lowerTranslation = prismatic->lowerTranslation;
    pjd.upperTranslation = prismatic->upperTranslation;

    pjd.enableMotor = prismatic->enableMotor;
    pjd.motorSpeed = prismatic->motorSpeed;
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

    auto t = component::getEntityComponent<component::Transform>(revolute->bodyA);

    b2RevoluteJointDef rjd;
    rjd.Initialize(bodyA, bodyB, b2Vec2(t->position.x, t->position.y));

    rjd.localAnchorA = b2Vec2(revolute->anchorA.x, revolute->anchorA.y);
    rjd.localAnchorB = b2Vec2(revolute->anchorB.x, revolute->anchorB.y);

    rjd.enableLimit = revolute->enableLimits;
    rjd.lowerAngle = revolute->lowerAngle;
    rjd.upperAngle = revolute->upperAngle;

    rjd.enableMotor = revolute->enableMotor;
    rjd.motorSpeed = revolute->motorSpeed;
    rjd.maxMotorTorque = revolute->maxMotorTorque;

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

    auto ta = component::getEntityComponent<component::Transform>(rigid->bodyA);
    auto tb = component::getEntityComponent<component::Transform>(rigid->bodyB);
    vec3 worldDir = tb->position - ta->position;
    vec3 localDir = inverse(ta->getWorldTransform(rigid->bodyA)) * worldDir;
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

std::vector<component::EntityId> Box2DEngine::rayCast(vec3 begin, vec3 end, bool onlyFirst) {
    RayCastCallback rc(onlyFirst);
    _world->RayCast(&rc, b2Vec2(begin.x, begin.y), b2Vec2(end.x, end.y));

    return rc.entities;
}

bool Box2DEngine::areColliding(component::EntityId eid0, component::EntityId eid1) {
    return _collisions.find(eid0) != _collisions.end() && _collisions[eid0].find(eid1) != _collisions[eid0].end();
}

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
    _bodies[_componentToEntity[rb2d]]->SetTransform(b2Vec2(position.x, position.y), angle);
}

void Box2DEngine::setLinearVelocity(component::RigidBody2D* rb2d, vec2 vel) {
    _bodies[_componentToEntity[rb2d]]->SetLinearVelocity(b2Vec2(vel.x, vel.y));
}

void Box2DEngine::setAngularVelocity(component::RigidBody2D* rb2d, float omega) { _bodies[_componentToEntity[rb2d]]->SetAngularVelocity(omega); }

void Box2DEngine::applyForce(component::RigidBody2D* rb2d, vec2 force, vec2 point, bool wake) {
    _bodies[_componentToEntity[rb2d]]->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), wake);
}

void Box2DEngine::applyForceToCenter(component::RigidBody2D* rb2d, vec2 force, bool wake) {
    _bodies[_componentToEntity[rb2d]]->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
}

void Box2DEngine::applyTorque(component::RigidBody2D* rb2d, float torque, bool wake) { _bodies[_componentToEntity[rb2d]]->ApplyTorque(torque, wake); }

} // namespace atta::physics
