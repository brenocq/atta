//--------------------------------------------------
// Atta Physics
// contact.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_CONTACT_H
#define ATTA_PHY_CONTACT_H

#include "simulator/math/math.h"
#include "simulator/physics/body.h"

namespace atta::phy
{
	// Contact between two bodies, resolving the contact means resolving their interpenetration
	// and applying impulse to keep them apart
	class Contact
	{
		public:
			// Bodies in contact, nullptr for contact with the scenery
			std::pair<Body*,Body*> objects;

			// Normal restitutional coefficient at the contact
			float restitution;

			// Depth of penetration at the contact
			float penetration;

			// Direction of the contact normal in world coordinates
			atta::vec3 contactNormal;
		
			void resolve(float dt);
			float calculateSeparatingVelocity() const;

		private:
			void resolveVelocity(float dt);
			void resolveInterpenetration(float dt);
	};
}
#endif// ATTA_PHY_CONTACT_H
