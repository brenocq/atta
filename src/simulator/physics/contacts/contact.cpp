//--------------------------------------------------
// Atta Physics
// contact.cpp
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "contact.h"

namespace atta::phy
{
	void Contact::resolve(float dt)
	{
		resolveVelocity(dt);
		resolveInterpenetration(dt);
	}

	float Contact::calculateSeparatingVelocity() const
	{
		vec3 relativeVel = bodies.first->getVelocity();
		if(bodies.second!=nullptr)
			relativeVel -= bodies.second->getVelocity();
		return relativeVel.dot(contactNormal);
	}

	void Contact::resolveVelocity(float dt)
	{
		float sepVel = calculateSeparatingVelocity();

		if(sepVel>0)
			// The contact is separating, no impulse is required
			return;

		// Calculate new separating velocity
		float newSepVel	= -sepVel*restitution;
		
		// Check velocity buildup due to acceleration only
		vec3 accCausedVelocity = bodies.first->getAcceleration();
		if(bodies.second !=nullptr) accCausedVelocity -= bodies.second->getAcceleration();
		float accCausedSepVel = accCausedVelocity.dot(contactNormal)*dt;
		if(accCausedSepVel<0)
		{
			newSepVel += restitution*accCausedSepVel;

			if(newSepVel<0)
				newSepVel = 0;
		}

		float deltaVel = newSepVel-sepVel;

		// Apply change in velocity in proportion to their inverse mass
		// Those with lower inverse mass (higher mass) get less change in velocity
		float totalInverseMass = bodies.first->getInverseMass();
		if(bodies.second!=nullptr)
			totalInverseMass += bodies.second->getInverseMass();

		// All bodies have infinity mass
		if(totalInverseMass<=0)
			return;

		float impulse = deltaVel/totalInverseMass; 

		// Total impulse per unit of inverse mass
		vec3 impulsePerIMass = contactNormal*impulse;


		// Apply impulses: In direction of the contact normal and proportional to the inverse mass
		bodies.first->setVelocity(bodies.first->getVelocity()+
				impulsePerIMass*bodies.first->getInverseMass());

		if(bodies.second!=nullptr)
			bodies.second->setVelocity(bodies.second->getVelocity()
					-impulsePerIMass*bodies.second->getInverseMass());
	}

	void Contact::resolveInterpenetration(float dt)
	{
		if(penetration<=0)
			return;

		// Apply change in position in proportion to their inverse mass
		// Those with lower inverse mass (higher mass) get less change in position
		float totalInverseMass = bodies.first->getInverseMass();
		if(bodies.second!=nullptr)
			totalInverseMass += bodies.second->getInverseMass();

		// All bodies have infinity mass
		if(totalInverseMass<=0)
			return;

		// Amount of penetration resolution per unit of inverse mass
		vec3 movePerIMass = contactNormal*(penetration/totalInverseMass);

		// Apply penetration resolution
		bodies.first->setPosition(bodies.first->getPosition()+
				movePerIMass*bodies.first->getInverseMass());

		if(bodies.second!=nullptr)
			bodies.second->setPosition(bodies.second->getPosition()-
					movePerIMass*bodies.second->getInverseMass());
	}
}
