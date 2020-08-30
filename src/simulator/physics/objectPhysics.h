//--------------------------------------------------
// Robot Simulator
// objectPhysics.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef OBJECT_PHYSICS_H
#define OBJECT_PHYSICS_H

#include "physicsEngine.h"
// Bullet
#include <btBulletDynamicsCommon.h>

class ObjectPhysics
{
	public:
		ObjectPhysics(btCollisionShape* bulletShape=nullptr, glm::vec3 position={0,0,0}, glm::vec3 rotation={0,0,0}, float mass=1.0);
		~ObjectPhysics();

		//---------- Getters ----------//
		btCollisionShape* getCollisionShape() const { return _bulletShape; }
		btRigidBody* getRigidBody() const { return _bulletBody; }
		btDefaultMotionState* getMotionState() const { return _bulletMotionState; }

		glm::vec3 getPosition() const;
		glm::vec3 getRotation() const;
		float getMass() const { return _mass; }

		//---------- Setters ----------//
		void setPosition(glm::vec3 position);
		void setRotation(glm::vec3 rotation);
		void setMass(float mass);

	private:
		float _mass;

		btCollisionShape* _bulletShape;
		btRigidBody* _bulletBody;
		btDefaultMotionState* _bulletMotionState;
};

#endif// OBJECT_PHYSICS_H
