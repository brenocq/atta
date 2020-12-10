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
		atta::vec3 relativeVel = objects.first->getVelocity();
		if(objects.second!=nullptr)
			relativeVel -= objects.second->getVelocity();
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
		atta::vec3 accCausedVelocity = objects.first->getAcceleration();
		if(objects.second !=nullptr) accCausedVelocity -= objects.second->getAcceleration();
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
		float totalInverseMass = objects.first->getInverseMass();
		if(objects.second!=nullptr)
			totalInverseMass += objects.second->getInverseMass();

		// All objects have infinity mass
		if(totalInverseMass<=0)
			return;

		float impulse = deltaVel/totalInverseMass; 

		// Total impulse per unit of inverse mass
		atta::vec3 impulsePerIMass = contactNormal*impulse;


		// Apply impulses: In direction of the contact normal and proportional to the inverse mass
		objects.first->setVelocity(objects.first->getVelocity()+
				impulsePerIMass*objects.first->getInverseMass());

		if(objects.second!=nullptr)
			objects.second->setVelocity(objects.second->getVelocity()
					-impulsePerIMass*objects.second->getInverseMass());
	}

	void Contact::resolveInterpenetration(float dt)
	{
		if(penetration<=0)
			return;

		// Apply change in position in proportion to their inverse mass
		// Those with lower inverse mass (higher mass) get less change in position
		float totalInverseMass = objects.first->getInverseMass();
		if(objects.second!=nullptr)
			totalInverseMass += objects.second->getInverseMass();

		// All objects have infinity mass
		if(totalInverseMass<=0)
			return;

		// Amount of penetration resolution per unit of inverse mass
		atta::vec3 movePerIMass = contactNormal*(penetration/totalInverseMass);

		// Apply penetration resolution
		objects.first->setPosition(objects.first->getPosition()+
				movePerIMass*objects.first->getInverseMass());

		if(objects.second!=nullptr)
			objects.second->setPosition(objects.second->getPosition()-
					movePerIMass*objects.second->getInverseMass());
	}
}
