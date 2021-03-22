//--------------------------------------------------
// Atta Physics
// physicsEngine.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_PHYSICS_ENGINE_H
#define ATTA_PHYSICS_PHYSICS_ENGINE_H

#include <vector>
#include <atta/physics/body.h>
#include <atta/math/math.h>
#include <atta/core/accelerator.h>
#include <atta/physics/forces/forceGenerator.h>
#include <atta/physics/contacts/contactGenerator.h>
#include <atta/physics/contacts/contactResolver.h>

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
			std::shared_ptr<ContactGenerator> _contactGenerator;

	};
}
#endif// ATTA_PHYSICS_PHYSICS_ENGINE_H
