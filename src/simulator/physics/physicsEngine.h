//--------------------------------------------------
// Robot PhysicsEngine
// physicsEngine.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <vector>
#include "glm.h"
#include "objectPhysics.h"
#include "forces/forceGenerator.h"

class PhysicsEngine
{
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void stepPhysics(float dt);
		void addObjectPhysics(ObjectPhysics* objectPhysics);

		bool raycast(glm::vec3 startPosition, glm::vec3 direction);
		//---------- Getters ----------//

		//------- Static helpers ------//
		static glm::vec3 getMouseClickRay(int x, int y, int width, int height, glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camUp);
	private:
		std::vector<ObjectPhysics*> _objectsPhysics;
		ForceGenerator* _forceGenerator;

};

#endif// PHYSICS_ENGINE_H
