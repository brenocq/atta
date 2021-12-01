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

    void Box2DEngine::start()
	{
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

			if(t && rb2d && (box2d || circle2d))
			{
				//----- Create body -----//
				b2BodyDef bodyDef;
				LOG_DEBUG("Box2DEngine", "Creating body $0 -> $1", entity, rb2d->type);
				if(rb2d->type == RigidBody2DComponent::DYNAMIC)
  					bodyDef.type = b2_dynamicBody;
				else if(rb2d->type == RigidBody2DComponent::KINEMATIC)
  					bodyDef.type = b2_kinematicBody;
				else if(rb2d->type == RigidBody2DComponent::STATIC)
  					bodyDef.type = b2_staticBody;

  				bodyDef.position.Set(t->position.x, t->position.y);
  				bodyDef.angle = -t->orientation.toEuler().z;
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
					polygonShape.SetAsBox(box2d->size.x/2.0f, box2d->size.y/2.0f);
					fixtureDef.shape = &polygonShape;
				}
				else if(circle2d)
				{
					b2CircleShape circle;
					circle.m_radius = circle2d->radius;
					fixtureDef.shape = &circle;
				}

				// Material properties
				fixtureDef.density = rb2d->density;
				fixtureDef.friction = rb2d->friction;
				fixtureDef.restitution = rb2d->restitution;
				fixtureDef.restitutionThreshold = 0.1f;
				body->CreateFixture(&fixtureDef);

				LOG_DEBUG("Box2DEngine", "Entity $0 has rigid body 2D!", entity);
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

			if(t)
			{
				// Update position
				b2Vec2 pos = p.second->GetPosition();
				t->position = { pos.x, pos.y, 0.0f };

				// Update angle
				float angle = -p.second->GetAngle();
				quat q;
				q.fromEuler({ 0, 0, angle });
				t->orientation = q;
			}
		}
	}

    void Box2DEngine::stop()
	{
		_bodies.clear();
		_world.reset();
	}
}
