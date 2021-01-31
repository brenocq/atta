//--------------------------------------------------
// Atta Physics
// contactResolver.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_CONTACT_RESOLVER_H
#define ATTA_PHY_CONTACT_RESOLVER_H

#include <vector>
#include "contact.h"

namespace atta::phy
{
	// Performs the contact resolution routine
	class ContactResolver
	{
		public:
			ContactResolver();
			~ContactResolver();

			void resolveContacts(std::vector<Contact> &contacts, float dt);

		private:
			void prepareContacts(std::vector<Contact> &contacts, float dt);
			void adjustPositions(std::vector<Contact> &contacts, float dt);
			void adjustVelocities(std::vector<Contact> &contacts, float dt);

			unsigned int _positionIterations;
			float _positionEpsilon;
			unsigned int _velocityIterations;
			float _velocityEpsilon;
	};
}

#endif// ATTA_PHY_CONTACT_RESOLVER_H
