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
		struct RayResult {
			btRigidBody* body;
			glm::vec3 hitPoint;
		};

		PhysicsEngine();
		~PhysicsEngine();

		void addRigidBody(btRigidBody* rigidBody);
		void stepSimulation(float dt);

		bool raycast(glm::vec3 startPosition, glm::vec3 direction, RayResult& output);

		static glm::vec3 getMouseClickRay(int x, int y, int width, int height, glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camUp);
		static btVector3 glm2bt(glm::vec3 vec);
		static glm::vec3 bt2glm(btVector3 vec);
		static btQuaternion glm2bt(glm::quat quat);
		static glm::quat bt2glm(btQuaternion quat);
	private:
		// Core Bullet components
		btBroadphaseInterface* _bulletBroadphase;
		btCollisionConfiguration* _bulletCollisionConfiguration;
		btCollisionDispatcher* _bulletDispatcher;
		btConstraintSolver* _bulletSolver;
		btDynamicsWorld* _bulletWorld;
};

#endif// PHYSICS_ENGINE_H
