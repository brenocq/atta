//--------------------------------------------------
// Robot PhysicsEngine
// physicsEngine.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <vector>
#include "objectPhysics.h"
#include "simulator/math/math.h"
#include "forces/forceGenerator.h"
#include "collisions/contactGenerator.h"
#include "collisions/contactResolver.h"

class PhysicsEngine
{
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void stepPhysics(float dt);
		void addObjectPhysics(ObjectPhysics* objectPhysics);

		bool raycast(atta::vec3 startPosition, atta::vec3 direction);
		//---------- Getters ----------//

		//------- Static helpers ------//
		static atta::vec3 getMouseClickRay(int x, int y, int width, int height, atta::vec3 camPos, atta::vec3 camForward, atta::vec3 camUp);
	private:
		std::vector<ObjectPhysics*> _objectsPhysics;
		ForceGenerator* _forceGenerator;
		ContactResolver* _contactResolver;
		std::vector<ContactGenerator> _contactGenerators;
		std::vector<Contact> _contacts;

};

#endif// PHYSICS_ENGINE_H
