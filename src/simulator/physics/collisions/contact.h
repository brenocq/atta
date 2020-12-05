//--------------------------------------------------
// Robot Simulator
// contact.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CONTACT_H
#define CONTACT_H

#include "glm.h"
#include "../objectPhysics.h"

// Contact between two objects, resolving the contact means resolving their interpenetration
// and applying impulse to keep them apart
class Contact
{
	public:
		// Objects in contact, nullptr for contact with the scenery
		std::pair<ObjectPhysics*,ObjectPhysics*> objects;

		// Normal restitutional coefficient at the contact
		float restitution;

		// Depth of penetration at the contact
		float penetration;

		// Direction of the contact normal in world coordinates
		glm::vec3 contactNormal;
	
		void resolve(float dt);
		float calculateSeparatingVelocity() const;

	private:
		void resolveVelocity(float dt);
		void resolveInterpenetration(float dt);
};

#endif// CONTACT_H
