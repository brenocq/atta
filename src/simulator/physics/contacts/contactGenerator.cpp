//--------------------------------------------------
// Atta Physics
// contactGenerator.cpp
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "contactGenerator.h"

namespace atta::phy
{
	ContactGenerator::ContactGenerator():
		_maxContacts(100), _contactsLeft(100)
	{

	}


	//-------------------- Sphere-Sphere --------------------//
	unsigned ContactGenerator::generateContact(std::shared_ptr<SphereShape> s1, std::shared_ptr<SphereShape> s2)
	{
		if(_contactsLeft<=0) return 0;

		// TODO Displaced sphere position can change with body rotation
		vec3 pos1 = s1->getPosition()+s1->getBody()->getPosition();
		vec3 pos2 = s2->getPosition()+s2->getBody()->getPosition();
		float radSum = s1->getRadius()+s2->getRadius();

		vec3 middleLine = pos2-pos1;
		float dist = middleLine.length();

		if(dist <= 0.0f || dist > radSum)
			return 0;

		vec3 normal = normalize(middleLine);

		Contact contact;
		contact.contactPoint = pos1+middleLine*0.5f;
		contact.contactNormal = normal;
		contact.penetration = radSum-dist;
		contact.bodies = std::make_pair(s1->getBody(), s2->getBody());

		_contacts.push_back(contact);
		_contactsLeft--;

		return 1;
	}

	//-------------------- Sphere-HalfSpace --------------------//
	unsigned ContactGenerator::generateContact(std::shared_ptr<HalfSpaceShape> p, std::shared_ptr<SphereShape> s)
	{
		return generateContact(s, p);
	}

	unsigned ContactGenerator::generateContact(std::shared_ptr<SphereShape> s, std::shared_ptr<HalfSpaceShape> p)
	{
		if(_contactsLeft<=0) return 0;

		vec3 posS = s->getPosition()+s->getBody()->getPosition();
		vec3 planeNormal = p->getNormal();

		float distance = dot(posS, planeNormal) - p->getOffset();

		if(distance >= s->getRadius())
			return 0;

		Contact contact;
		contact.contactPoint = posS - planeNormal * distance;
		contact.contactNormal = planeNormal;
		contact.penetration = distance-s->getRadius();
		contact.bodies = std::make_pair(s->getBody(), p->getBody());

		_contacts.push_back(contact);
		_contactsLeft--;

		return 1;
	}

	//-------------------- Sphere-Plane --------------------//
	unsigned ContactGenerator::generateContact(std::shared_ptr<PlaneShape> p, std::shared_ptr<SphereShape> s)
	{
		return generateContact(s, p);
	}

	unsigned ContactGenerator::generateContact(std::shared_ptr<SphereShape> s, std::shared_ptr<PlaneShape> p)
	{
		if(_contactsLeft<=0) return 0;

		vec3 posS = s->getPosition()+s->getBody()->getPosition();
		vec3 normal = p->getNormal();

		float distance = dot(posS, normal) - p->getOffset();

		if(distance*distance  >= s->getRadius()*s->getRadius())
			return 0;

		float penetration = -distance;

		// Check which side of the plane
		if(distance < 0)
		{
			normal*=-1;
			penetration = -penetration;
		}
		penetration += s->getRadius();

		Contact contact;
		contact.contactPoint = posS - normal * distance;
		contact.contactNormal = normal;
		contact.penetration = penetration;
		contact.bodies = std::make_pair(s->getBody(), p->getBody());

		_contacts.push_back(contact);
		_contactsLeft--;

		return 1;
	}

	//-------------------- Box-HalfSpace --------------------//
	unsigned ContactGenerator::generateContact(std::shared_ptr<BoxShape> b, std::shared_ptr<HalfSpaceShape> p)
	{
		return generateContact(p, b);
	}

	unsigned ContactGenerator::generateContact(std::shared_ptr<HalfSpaceShape> p, std::shared_ptr<BoxShape> b)
	{
		if(_contactsLeft<=0) return 0;

		// TODO check intersection

		std::vector<vec3> vertices = {	vec3(0.5, 0.5, 0.5), vec3(-0.5, 0.5, 0.5), 
										vec3(-0.5, 0.5, -0.5), vec3(0.5, 0.5, -0.5), 
										vec3(0.5, -0.5, 0.5), vec3(-0.5, -0.5, 0.5), 
										vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, -0.5)};

		// Calculate transform matrix
		mat4 modelShape = posOriScale(b->getPosition(), b->getOrientation(), b->getScale());
		Body* body = b->getBody();
		mat4 modelBody = posOri(body->getPosition(), body->getOrientation());
		mat4 model = modelBody*modelShape;

		unsigned contactsUsed = 0;
		// Generate contacts (each box vertex with the half space)
		for(auto& vertex : vertices)
		{
			vertex = vec3(model*vec4(vertex,0));
			float vertexDistance = dot(vertex, p->getNormal());

			if(vertexDistance <= p->getOffset())// Behind half space
			{
				float penetration = p->getOffset()-vertexDistance;

				Contact contact;
				contact.contactPoint = vertex + penetration*p->getNormal();// Middle point between vertex and plane
				contact.contactNormal = p->getNormal();
				contact.penetration = penetration;
				contact.bodies = std::make_pair(b->getBody(), p->getBody());

				_contacts.push_back(contact);
				_contactsLeft--;
				contactsUsed++;
				if(_contactsLeft==0) return contactsUsed;
			}
		}
		return contactsUsed;
	}

	//-------------------- Box-Sphere --------------------//
	unsigned ContactGenerator::generateContact(std::shared_ptr<BoxShape> b, std::shared_ptr<SphereShape> s)
	{
		return generateContact(s, b);
	}

	unsigned ContactGenerator::generateContact(std::shared_ptr<SphereShape> s, std::shared_ptr<BoxShape> b)
	{
		// Transform sphere center to box local space

		// Calculate vertex-face, edge-face, face-face
		
		// Create contact in world space

		return 0;
	}

	//-------------------- Box-Box --------------------//
	unsigned ContactGenerator::generateContact(std::shared_ptr<BoxShape> b1, std::shared_ptr<BoxShape> b2)
	{
		if(_contactsLeft<=0) return 0;

		return 0;
	}
}
