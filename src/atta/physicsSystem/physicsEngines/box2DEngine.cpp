//--------------------------------------------------
// Atta Physics System
// box2DEngine.cpp
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physicsSystem/physicsEngines/box2DEngine.h>
#include <atta/componentSystem/components/rigidBody2DComponent.h>
#include <atta/componentSystem/components/transformComponent.h>

namespace atta
{
	Box2DEngine::Box2DEngine():
		_world(b2Vec2(0.0f, -10.0f))
	{
		std::vector<EntityId> entities = ComponentManager::getNoPrototypeView();
		for(EntityId entity : entities)
		{
			auto rb2d = ComponentManager::getEntityComponent<RigidBody2DComponent>(entity);
			if(rb2d)
			{
				//----- Create body -----//
				b2BodyDef bodyDef;
				if(rb2d->type == RigidBody2DComponent::DYNAMIC)
  					bodyDef.type = b2_dynamicBody;
				else if(rb2d->type == RigidBody2DComponent::KINEMATIC)
  					bodyDef.type = b2_kinematicBody;
				else if(rb2d->type == RigidBody2DComponent::STATIC)
  					bodyDef.type = b2_staticBody;

  				bodyDef.position.Set(rb2d->position.x, rb2d->position.y);
  				bodyDef.angle = rb2d->angle;
  				bodyDef.linearVelocity.Set(rb2d->linearVelocity.x, rb2d->linearVelocity.y);
  				bodyDef.angularVelocity = rb2d->angularVelocity;
  				bodyDef.linearDamping = rb2d->linearDamping;
  				bodyDef.angularDamping = rb2d->angularDamping;
  				bodyDef.allowSleep = rb2d->allowSleep;
  				bodyDef.awake = rb2d->awake;
  				bodyDef.fixedRotation = rb2d->fixedRotation;

				b2Body* body = _world.CreateBody(&bodyDef);
				_bodies.push_back(std::make_pair(entity, body));

				//----- Attach collider -----//
				// Create shape
				b2PolygonShape dynamicBox;
				dynamicBox.SetAsBox(0.5f, 0.5f);

				// Material properties
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &dynamicBox;
				fixtureDef.density = 1.0f;
				fixtureDef.friction = 0.3f;
				body->CreateFixture(&fixtureDef);

				LOG_DEBUG("Box2DEngine", "Entity $0 has rigid body 2D!", entity);
			}
		}
	}

	Box2DEngine::~Box2DEngine()
	{

	}

    void Box2DEngine::step(float dt)
	{
		int velocityIterations = 6;
		int positionIterations = 2;
		_world.Step(dt, velocityIterations, positionIterations);
		
		for(auto p : _bodies)
		{
			auto t = ComponentManager::getEntityComponent<TransformComponent>(p.first);

			if(t)
			{
				// Update position
				b2Vec2 pos = p.second->GetPosition();
				t->position = { pos.x, pos.y, 0.0f };

				// Update angle
				float angle = p.second->GetAngle();
				quat q;
				q.fromEuler({0, 0, angle });
				t->orientation = q;
			}
		}
	}
}
