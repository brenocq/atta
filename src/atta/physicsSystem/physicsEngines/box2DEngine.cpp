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
		for(EntityId entity : entities)
		{
			auto t = ComponentManager::getEntityComponent<TransformComponent>(entity);
			auto rb2d = ComponentManager::getEntityComponent<RigidBody2DComponent>(entity);
			auto box2d = ComponentManager::getEntityComponent<BoxCollider2DComponent>(entity);
			auto circle2d = ComponentManager::getEntityComponent<CircleCollider2DComponent>(entity);

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
				_bodies.push_back(std::make_pair(entity, body));

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
				// Get parent world transform
				vec3 positionP, scaleP;
				quat orientationP;
				if(tp)
				{
					mat4 m = tp->getWorldTransform(r->getParent());
					m.getPosOriScale(positionP, orientationP, scaleP);
				}

				// Update position
				b2Vec2 pos = p.second->GetPosition();
				t->position = { pos.x-positionP.x, pos.y-positionP.y, 0.0f };

				// Update angle
				float angle = -p.second->GetAngle();
				float angleP = tp ? -orientationP.toEuler().z : 0;
				quat q;
				q.fromEuler({ 0, 0, angle-angleP });
				t->orientation = q;
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
