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

	void ContactGenerator::clearContacts()
	{
		_contacts.clear();
		_contactsLeft = _maxContacts;
	}

	// Cast shape to right
	unsigned ContactGenerator::testContact(std::shared_ptr<Shape> s1, std::shared_ptr<Shape> s2)
	{
		switch((100*s1->getType() + s2->getType()))
		{
			case SPHERE_SPHERE_PAIR:
				testContact(std::static_pointer_cast<SphereShape>(s1), std::static_pointer_cast<SphereShape>(s2)); break;
			//case SPHERE_BOX_PAIR:
			case SPHERE_HALF_SPACE_PAIR:
				testContact(std::static_pointer_cast<SphereShape>(s1), std::static_pointer_cast<HalfSpaceShape>(s2)); break;
			//case SPHERE_PLANE_PAIR:
			//	testContact(std::static_pointer_cast<SphereShape>(s1), std::static_pointer_cast<PlaneShape>(s2)); break;
			//case SPHERE_CYLINDER_PAIR:
			//case SPHERE_CAPSULE_PAIR:

			//case BOX_SPHERE_PAIR:
			//case BOX_BOX_PAIR:
			case BOX_HALF_SPACE_PAIR:
				testContact(std::static_pointer_cast<BoxShape>(s1), std::static_pointer_cast<HalfSpaceShape>(s2)); break;
			//case BOX_PLANE_PAIR:
			//case BOX_CYLINDER_PAIR:
			//case BOX_CAPSULE_PAIR:

			case HALF_SPACE_SPHERE_PAIR:
				testContact(std::static_pointer_cast<HalfSpaceShape>(s1), std::static_pointer_cast<SphereShape>(s2)); break;
			case HALF_SPACE_BOX_PAIR:
				testContact(std::static_pointer_cast<HalfSpaceShape>(s1), std::static_pointer_cast<BoxShape>(s2)); break;
			//case HALF_SPACE_HALF_SPACE_PAIR:
			//case HALF_SPACE_PLANE_PAIR:
			//case HALF_SPACE_CYLINDER_PAIR:
			//case HALF_SPACE_CAPSULE_PAIR:

			//case PLANE_SPHERE_PAIR:
			//case PLANE_BOX_PAIR:
			//case PLANE_HALF_SPACE_PAIR:
			//case PLANE_PLANE_PAIR:
			//case PLANE_CYLINDER_PAIR:
			//case PLANE_CAPSULE_PAIR:

			//case CYLINDER_SPHERE_PAIR:
			//case CYLINDER_BOX_PAIR:
			//case CYLINDER_HALF_SPACE_PAIR:
			//case CYLINDER_PLANE_PAIR:
			//case CYLINDER_CYLINDER_PAIR:
			//case CYLINDER_CAPSULE_PAIR:

			//case CAPSULE_SPHERE_PAIR:
			//case CAPSULE_BOX_PAIR:
			//case CAPSULE_HALF_SPACE_PAIR:
			//case CAPSULE_PLANE_PAIR:
			//case CAPSULE_CYLINDER_PAIR:
			//case CAPSULE_CAPSULE_PAIR:
			default:
				Log::warning("ContactGenerator", "Collision between uninplemented shapes! ($0_SHAPE and $1_SHAPE)", 
						s1->getTypeName(), s2->getTypeName());
		}

		return 0;
	}

	//-------------------- Sphere-Sphere --------------------//
	unsigned ContactGenerator::testContact(std::shared_ptr<SphereShape> s1, std::shared_ptr<SphereShape> s2)
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
		contact.setBodyData(s1->getBody(), s2->getBody());

		_contacts.push_back(contact);
		_contactsLeft--;

		return 1;
	}

	//-------------------- Sphere-HalfSpace --------------------//
	unsigned ContactGenerator::testContact(std::shared_ptr<HalfSpaceShape> p, std::shared_ptr<SphereShape> s)
	{
		return testContact(s, p);
	}

	unsigned ContactGenerator::testContact(std::shared_ptr<SphereShape> s, std::shared_ptr<HalfSpaceShape> p)
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
		contact.setBodyData(s->getBody(), p->getBody());

		_contacts.push_back(contact);
		_contactsLeft--;

		return 1;
	}

	//-------------------- Sphere-Plane --------------------//
	//unsigned ContactGenerator::testContact(std::shared_ptr<PlaneShape> p, std::shared_ptr<SphereShape> s)
	//{
	//	return testContact(s, p);
	//}

	//unsigned ContactGenerator::testContact(std::shared_ptr<SphereShape> s, std::shared_ptr<PlaneShape> p)
	//{
	//	if(_contactsLeft<=0) return 0;

	//	vec3 posS = s->getPosition()+s->getBody()->getPosition();
	//	vec3 normal = p->getNormal();

	//	float distance = dot(posS, normal) - p->getOffset();

	//	if(distance*distance  >= s->getRadius()*s->getRadius())
	//		return 0;

	//	float penetration = -distance;

	//	// Check which side of the plane
	//	if(distance < 0)
	//	{
	//		normal*=-1;
	//		penetration = -penetration;
	//	}
	//	penetration += s->getRadius();

	//	Contact contact;
	//	contact.contactPoint = posS - normal * distance;
	//	contact.contactNormal = normal;
	//	contact.penetration = penetration;
	//	contact.setBodyData(s->getBody(), p->getBody());

	//	_contacts.push_back(contact);
	//	_contactsLeft--;

	//	return 1;
	//}

	//-------------------- Box-HalfSpace --------------------//
	unsigned ContactGenerator::testContact(std::shared_ptr<BoxShape> b, std::shared_ptr<HalfSpaceShape> p)
	{
		return testContact(p, b);
	}

	unsigned ContactGenerator::testContact(std::shared_ptr<HalfSpaceShape> p, std::shared_ptr<BoxShape> b)
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
		mat4 modelBody = posOriScale(body->getPosition(), body->getOrientation(), {1,1,1});
		mat4 model = modelBody*modelShape;
		//Log::debug("ContactGen", "model: $0 from $1 $2", modelBody.toString(), body->getPosition().toString(), degrees(quatToEuler(body->getOrientation())).toString());

		unsigned contactsUsed = 0;
		// Generate contacts (each box vertex with the half space)
		for(auto& vertex : vertices)
		{
			vertex = vec3(model*vec4(vertex,1));
			//Log::debug("ContactGen", "dot: $0 * $1 = $2",vertex.toString(), p->getNormal().toString(), dot(vertex, p->getNormal()));
			float vertexDistance = dot(vertex, p->getNormal());

			if(vertexDistance <= p->getOffset())// Behind half space
			{
				float penetration = p->getOffset()-vertexDistance;

				Contact contact;
				contact.contactPoint = vertex + penetration*p->getNormal();// Vertex point projected to the plane
				contact.contactNormal = p->getNormal();
				contact.penetration = penetration;
				contact.friction = 0.0f;
				contact.restitution = 0.0;
				contact.setBodyData(b->getBody(), nullptr);
				//Log::debug("ContactGen", "Contact: $0", contact.toString());

				_contacts.push_back(contact);
				_contactsLeft--;
				contactsUsed++;
				if(_contactsLeft==0) return contactsUsed;
			}
		}
		if(contactsUsed==2)
			Log::debug("ContactGen", "Contacts:\n$0\n$1", _contacts[_contacts.size()-1].toString(), _contacts[_contacts.size()-2].toString());

		return contactsUsed;
	}

	//-------------------- Box-Sphere --------------------//
	unsigned ContactGenerator::testContact(std::shared_ptr<BoxShape> b, std::shared_ptr<SphereShape> s)
	{
		return testContact(s, b);
	}

	unsigned ContactGenerator::testContact(std::shared_ptr<SphereShape> s, std::shared_ptr<BoxShape> b)
	{
		// Transform sphere center to box local space

		// Calculate vertex-face, edge-face, face-face
		
		// Create contact in world space

		return 0;
	}

	//-------------------- Box-Box --------------------//
	unsigned ContactGenerator::testContact(std::shared_ptr<BoxShape> b1, std::shared_ptr<BoxShape> b2)
	{
		if(_contactsLeft<=0) return 0;

		return 0;
	}
}
