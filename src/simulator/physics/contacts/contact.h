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
		friend class ContactResolver;
		public:
			// Bodies in contact, nullptr for contact with the scenery
			Body* bodies[2];

			// Point of contact in world coordinates
			vec3 contactPoint;

			// Direction of the contact normal in world coordinates
			vec3 contactNormal;

			// Normal restitutional coefficient at the contact
			float restitution;

			// Depth of penetration at the contact
			float penetration;
		
			void setBodyData(Body* b0, Body* b1);
		protected:
			//----- Data stored to resolve the contact -----//
			// Transform matrix from contact's frame of reference to world coordinates
			mat3 contactToWorld;

			// Closing velocity at the point of contact
			vec3 contactVelocity;

			// Required change in velocity for this contact to be resolved
			float desiredDeltaVelocity;

			// world-space position of the contact point relative to the center of each body
			vec3 relativeContactPosition[2];

		protected:
			//----- Prepare data -----//
			// Populate data to resolve the contact
			void calculateInternals(float dt);

			void swapBodies();

			void calculateContactBasis();

			vec3 calculateLocalVelocity(int bodyIndex, float dt);

			void calculateDesiredDeltaVelocity(float dt);

			//----- Contact resolving -----//
			// Performs an inertia weighted penetration resolution of this contact alone
			void applyPositionChange(vec3 linearChange[2], vec3 angularChange[2], float penetration);
			void applyVelocityChange(vec3 velocityChange[2], vec3 rotationChange[2]);
	};
}
#endif// ATTA_PHY_CONTACT_H
