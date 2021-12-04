//--------------------------------------------------
// Atta Physics System
// box2DEngine.cpp
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physicsSystem/physicsEngines/box2DEngine.h>
#include <atta/componentSystem/components/components.h>

namespace atta
{
    Box2DEngine::Box2DEngine():
        PhysicsEngine(PhysicsEngine::BOX2D_ENGINE)
    {
    }

    Box2DEngine::~Box2DEngine()
    {
        if(_running)
            stop();
    }

    void Box2DEngine::start()
    {
        _running = true;
        _world = std::make_shared<b2World>(b2Vec2(0.0f, -10.0f));

        std::vector<EntityId> entities = ComponentManager::getNoPrototypeView();
        //---------- Create rigid bodies ----------//
        for(EntityId entity : entities)
        {
            auto t = ComponentManager::getEntityComponent<TransformComponent>(entity);
            auto rb2d = ComponentManager::getEntityComponent<RigidBody2DComponent>(entity);
            auto box2d = ComponentManager::getEntityComponent<BoxCollider2DComponent>(entity);
            auto circle2d = ComponentManager::getEntityComponent<CircleCollider2DComponent>(entity);
            
            if(!rb2d) continue;

            if(!t && rb2d)
            {
                LOG_WARN("box2DEngine", "Entity [w]$0[] is a rigid body but does not have a transform component", entity);
                continue;
            }

            if(rb2d && !(box2d || circle2d))
            {
                LOG_WARN("box2DEngine", "Entity [w]$0[] is a rigid body but does not have any collider component", entity);
                continue;
            }

            if(box2d && circle2d)
            {
                LOG_WARN("box2DEngine", "Entity [w]$0[] must have only one collider", entity);
                continue;
            }

            vec3 position, scale;
            quat orientation;
            mat4 m = t->getWorldTransform(entity);
            m.getPosOriScale(position, orientation, scale);

            if(t && rb2d && (box2d || circle2d))
            {
                //----- Create body -----//
                b2BodyDef bodyDef;
                if(rb2d->type == RigidBody2DComponent::DYNAMIC)
                    bodyDef.type = b2_dynamicBody;
                else if(rb2d->type == RigidBody2DComponent::KINEMATIC)
                    bodyDef.type = b2_kinematicBody;
                else if(rb2d->type == RigidBody2DComponent::STATIC)
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

                b2Body* body = _world->CreateBody(&bodyDef);
                _bodies[entity] = body;

                //----- Attach collider -----//
                b2FixtureDef fixtureDef;
                b2PolygonShape polygonShape;
                // Create shape
                if(box2d)
                {
                    polygonShape.SetAsBox(scale.x * box2d->size.x/2.0f, scale.y * box2d->size.y/2.0f);
                    fixtureDef.shape = &polygonShape;
                }
                else if(circle2d)
                {
                    b2CircleShape circle;
                    circle.m_radius = scale.x * circle2d->radius;
                    fixtureDef.shape = &circle;
                }

                // Material properties
                fixtureDef.density = rb2d->density;
                fixtureDef.friction = rb2d->friction;
                fixtureDef.restitution = rb2d->restitution;
                fixtureDef.restitutionThreshold = 0.1f;
                body->CreateFixture(&fixtureDef);
            }
        }

        //---------- Create joints ----------//
        for(EntityId entity : entities)
        {
            auto prismatic = ComponentManager::getEntityComponent<PrismaticJointComponent>(entity);
            auto revolute = ComponentManager::getEntityComponent<RevoluteJointComponent>(entity);
            auto rigid = ComponentManager::getEntityComponent<RigidJointComponent>(entity);

            if(prismatic)
            {
                b2Body* bodyA;
                b2Body* bodyB;
                if(_bodies.find(prismatic->bodyA) == _bodies.end() || _bodies.find(prismatic->bodyB) == _bodies.end())
                {
                    LOG_WARN("Box2DEngine", "Trying to create joint with entity that is not a rigid body");
                    goto finishCreatePrismatic;
                }
                else
                {
                    bodyA = _bodies[prismatic->bodyA];
                    bodyB = _bodies[prismatic->bodyB];
                }

                auto t = ComponentManager::getEntityComponent<TransformComponent>(prismatic->bodyA);

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
finishCreatePrismatic:

            if(revolute)
            {
                b2Body* bodyA;
                b2Body* bodyB;
                if(_bodies.find(revolute->bodyA) == _bodies.end() || _bodies.find(revolute->bodyB) == _bodies.end())
                {
                    LOG_WARN("Box2DEngine", "Trying to create joint with entity that is not a rigid body");
                    goto finishCreateRevolute;
                }
                else
                {
                    bodyA = _bodies[revolute->bodyA];
                    bodyB = _bodies[revolute->bodyB];
                }

                auto t = ComponentManager::getEntityComponent<TransformComponent>(revolute->bodyA);

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
finishCreateRevolute:

            if(rigid)
            {
                b2Body* bodyA;
                b2Body* bodyB;
                if(_bodies.find(rigid->bodyA) == _bodies.end() || _bodies.find(rigid->bodyB) == _bodies.end())
                {
                    LOG_WARN("Box2DEngine", "Trying to create joint with entity that is not a rigid body");
                    goto finishCreateRigid;
                }
                else
                {
                    bodyA = _bodies[rigid->bodyA];
                    bodyB = _bodies[rigid->bodyB];
                }

                auto ta = ComponentManager::getEntityComponent<TransformComponent>(rigid->bodyA);
                auto tb = ComponentManager::getEntityComponent<TransformComponent>(rigid->bodyB);
                vec3 worldDir = tb->position-ta->position;
                vec3 localDir = inverse(ta->getWorldTransform(rigid->bodyA))*worldDir;
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

finishCreateRigid:
            {}
        }
    }

    void Box2DEngine::step(float dt)
    {
        int velocityIterations = 6;
        int positionIterations = 2;
        _world->Step(dt, velocityIterations, positionIterations);

        for(auto p : _bodies)
        {
            auto t = ComponentManager::getEntityComponent<TransformComponent>(p.first);
            auto r = ComponentManager::getEntityComponent<RelationshipComponent>(p.first);
            TransformComponent* tp = nullptr;
            if(r && r->getParent() >= 0)
                tp = ComponentManager::getEntityComponent<TransformComponent>(r->getParent()); 

            if(t)
            {
                // Get old transform
                vec3 position;
                quat orientation;
                vec3 scale;
                mat4 m = t->getWorldTransform(p.first);
                m.getPosOriScale(position, orientation, scale);
                // Calculate new transform (after physics step)
                b2Vec2 pos = p.second->GetPosition();
                position = { pos.x, pos.y, 0.0f };
                orientation.fromEuler({ 0, 0, -p.second->GetAngle() });

                // Update pos/ori/scale to be local to parent
                if(tp)
                {
                    mat4 childTransform;
                    childTransform.setPosOriScale(position, orientation, scale);

                    mat4 parentTransform = tp->getWorldTransform(r->getParent());
                    mat4 finalTransform = inverse(parentTransform) * childTransform;
                    finalTransform.getPosOriScale(position, orientation, scale);
                }

                // Update
                t->position = position;
                t->orientation = orientation;
                t->scale = scale;
            }
        }
    }

    void Box2DEngine::stop()
    {
        _running = false;
        _bodies.clear();
        _world.reset();
    }
}
