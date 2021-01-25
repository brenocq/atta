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
#include "simulator/core/accelerator.h"
#include "forces/forceGenerator.h"
#include "collisions/contactGenerator.h"
#include "collisions/contactResolver.h"

namespace atta::phy
{
	class PhysicsEngine
	{
		public:
			struct CreateInfo {
				std::shared_ptr<Accelerator> accelerator;
			};

			PhysicsEngine(CreateInfo info);
			~PhysicsEngine();

			void stepPhysics(float dt);

		private:
			std::shared_ptr<Accelerator> _accelerator;

			std::vector<std::shared_ptr<Body>> _bodies;
			std::shared_ptr<ForceGenerator> _forceGenerator;
			std::shared_ptr<ContactResolver> _contactResolver;
			std::vector<ContactGenerator> _contactGenerators;
			std::vector<Contact> _contacts;

	};
}
#endif// ATTA_PHY_PHYSICS_ENGINE_H
