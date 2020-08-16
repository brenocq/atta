//--------------------------------------------------
// Robot PhysicsEngine
// physicsEngine.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "glm.h"
#include <glm/gtc/quaternion.hpp>
// Bullet
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

class PhysicsEngine
{
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void addRigidBody(btRigidBody* rigidBody);
		void stepSimulation(float dt);

		static btVector3 glm2bt(glm::vec3 vec);
		static glm::vec3 glm2bt(btVector3 vec);
		static btQuaternion glm2bt(glm::quat quat);
		static glm::quat glm2bt(btQuaternion quat);
	private:
		// Core Bullet components
		btBroadphaseInterface* _bulletBroadphase;
		btCollisionConfiguration* _bulletCollisionConfiguration;
		btCollisionDispatcher* _bulletDispatcher;
		btConstraintSolver* _bulletSolver;
		btDynamicsWorld* _bulletWorld;
};

#endif// PHYSICS_ENGINE_H
