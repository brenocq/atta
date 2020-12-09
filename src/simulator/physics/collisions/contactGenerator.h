//--------------------------------------------------
// Robot Simulator
// contactGenerator.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CONTACT_GENERATOR_H
#define CONTACT_GENERATOR_H

#include <vector>
#include "simulator/math/math.h"
#include "../objectPhysics.h"
#include "contact.h"

class ContactGenerator
{
	public:
		virtual std::vector<Contact> addContact(unsigned limit) const = 0;

	private:
};

#endif// CONTACT_GENERATOR_H
