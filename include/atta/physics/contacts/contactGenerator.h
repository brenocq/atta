//--------------------------------------------------
// Atta Physics
// contactGenerator.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_CONTACTS_CONTACT_GENERATOR_H
#define ATTA_PHYSICS_CONTACTS_CONTACT_GENERATOR_H

#include <vector>
#include "contact.h"
#include <atta/physics/shapes/shapes.h>

namespace atta::phy
{
	// Test shapes to generate contacts between them and store the contacts
	class ContactGenerator
	{
		public:
			ContactGenerator();

			void clearContacts();

			// Cast shape to right one
			unsigned testContact(std::shared_ptr<Shape> s1, std::shared_ptr<Shape> s2);

			//---------- Test contact between shapes ----------//
			unsigned testContact(std::shared_ptr<SphereShape> s1, std::shared_ptr<SphereShape> s2);

			unsigned testContact(std::shared_ptr<SphereShape> s, std::shared_ptr<HalfSpaceShape> p);
			unsigned testContact(std::shared_ptr<HalfSpaceShape> p, std::shared_ptr<SphereShape> s);

			//unsigned testContact(std::shared_ptr<SphereShape> s, std::shared_ptr<PlaneShape> p);
			//unsigned testContact(std::shared_ptr<PlaneShape> p, std::shared_ptr<SphereShape> s);

			unsigned testContact(std::shared_ptr<BoxShape> b, std::shared_ptr<HalfSpaceShape> p);
			unsigned testContact(std::shared_ptr<HalfSpaceShape> p, std::shared_ptr<BoxShape> b);

			// TODO
			unsigned testContact(std::shared_ptr<BoxShape> b, std::shared_ptr<SphereShape> s);
			unsigned testContact(std::shared_ptr<SphereShape> s, std::shared_ptr<BoxShape> b);

			// TODO
			unsigned testContact(std::shared_ptr<BoxShape> b1, std::shared_ptr<BoxShape> b2);

			//---------- Getters ----------//
			unsigned qtyContacts() { return _maxContacts-_contactsLeft; }
			std::vector<Contact>& getContacts() { return _contacts; }

		private:
			std::vector<Contact> _contacts;

			unsigned _maxContacts;// Maximum number of contacts to solve at each step
			unsigned _contactsLeft;
	};
}

#endif// ATTA_PHYSICS_CONTACTS_CONTACT_GENERATOR_H
