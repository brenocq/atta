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

namespace atta::phy
{
	class ContactGenerator
	{
		public:
			virtual std::vector<Contact> addContact(unsigned limit) const = 0;

		private:
	};
}

#endif// ATTA_PHY_CONTACT_GENERATOR_H
