//--------------------------------------------------
// Robot Simulator
// contactResolver.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CONTACT_RESOLVER_H
#define CONTACT_RESOLVER_H

#include <vector>
#include "glm.h"
#include "../objectPhysics.h"
#include "contact.h"

// Performs the contact resolution routine
class ContactResolver
{
	public:
		ContactResolver(unsigned int iterations);
		~ContactResolver();

		void resolveContacts(std::vector<Contact> contacts, float dt);

		//---------- Getters and Setters ----------//
		unsigned int getIterations() const { return _iterations; }
		void setIterations(unsigned int iterations) { _iterations=iterations; }
	
	private:
		unsigned int _iterations;
		unsigned int _iterationsUsed;// Performace tracking
};

#endif// CONTACT_RESOLVER_H
