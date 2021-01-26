//--------------------------------------------------
// Atta Physics
// contactResolver.cpp
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "contactResolver.h"
#include <limits>

namespace atta::phy
{
	ContactResolver::ContactResolver(unsigned int iterations):
		_iterations(iterations)
	{

	}

	ContactResolver::~ContactResolver()
	{

	}

	void ContactResolver::resolveContacts(std::vector<Contact> contacts, float dt)
	{
		_iterationsUsed = 0;
		while(_iterationsUsed < _iterations)
		{
			// Find contact with largest closing velocity
			float max = std::numeric_limits<float>::max();
			unsigned maxIndex =	contacts.size();
			for(unsigned i=0; i<contacts.size(); i++)
			{
				float sepVel = contacts[i].calculateSeparatingVelocity();
				if(sepVel<max && (sepVel<0 || contacts[i].penetration>0))
				{
					max = sepVel;
					maxIndex = i;
				}
			}

			
			if(maxIndex == contacts.size())
				break;// No contact to solve

			contacts[maxIndex].resolve(dt);
			_iterationsUsed++;
		}
	}
}
