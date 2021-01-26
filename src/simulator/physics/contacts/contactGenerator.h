//--------------------------------------------------
// Atta Physics
// contactGenerator.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_CONTACT_GENERATOR_H
#define ATTA_PHY_CONTACT_GENERATOR_H

#include <vector>
#include "contact.h"
#include "simulator/physics/shapes/shapes.h"

namespace atta::phy
{
	// Test shapes to generate contacts between them and store the contacts
	class ContactGenerator
	{
		public:
			ContactGenerator();

			unsigned generateContact(std::shared_ptr<SphereShape> s1, std::shared_ptr<SphereShape> s2);

			unsigned generateContact(std::shared_ptr<SphereShape> s, std::shared_ptr<HalfSpaceShape> p);
			unsigned generateContact(std::shared_ptr<HalfSpaceShape> p, std::shared_ptr<SphereShape> s);

			unsigned generateContact(std::shared_ptr<SphereShape> s, std::shared_ptr<PlaneShape> p);
			unsigned generateContact(std::shared_ptr<PlaneShape> p, std::shared_ptr<SphereShape> s);

			unsigned generateContact(std::shared_ptr<BoxShape> b, std::shared_ptr<HalfSpaceShape> p);
			unsigned generateContact(std::shared_ptr<HalfSpaceShape> p, std::shared_ptr<BoxShape> b);

			// TODO
			unsigned generateContact(std::shared_ptr<BoxShape> b, std::shared_ptr<SphereShape> s);
			unsigned generateContact(std::shared_ptr<SphereShape> s, std::shared_ptr<BoxShape> b);

			// TODO
			unsigned generateContact(std::shared_ptr<BoxShape> b1, std::shared_ptr<BoxShape> b2);

		private:
			std::vector<Contact> _contacts;

			unsigned _maxContacts;// Maximum number of contacts to solve at each step
			unsigned _contactsLeft;
	};
}

#endif// ATTA_PHY_CONTACT_GENERATOR_H
