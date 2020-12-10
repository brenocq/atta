//--------------------------------------------------
// Atta Physics
// physicsEngine.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_PHYSICS_ENGINE_H
#define ATTA_PHY_PHYSICS_ENGINE_H

#include <vector>
#include "body.h"
#include "simulator/math/math.h"
#include "forces/forceGenerator.h"
#include "collisions/contactGenerator.h"
#include "collisions/contactResolver.h"

namespace atta::phy
{
	class PhysicsEngine
	{
		public:
			PhysicsEngine();
			~PhysicsEngine();

			void stepPhysics(float dt);
			void addBody(Body* body);

			bool raycast(vec3 startPosition, atta::vec3 direction);
			//---------- Getters ----------//

			//------- Static helpers ------//
			static vec3 getMouseClickRay(int x, int y, int width, int height, vec3 camPos, vec3 camForward, vec3 camUp);
		private:
			std::vector<Body*> _bodies;
			ForceGenerator* _forceGenerator;
			ContactResolver* _contactResolver;
			std::vector<ContactGenerator> _contactGenerators;
			std::vector<Contact> _contacts;

	};
}
#endif// ATTA_PHY_PHYSICS_ENGINE_H
