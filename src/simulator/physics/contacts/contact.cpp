//--------------------------------------------------
// Atta Physics
// contact.cpp
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "contact.h"

namespace atta::phy
{
	void Contact::setBodyData(Body* b0, Body* b1)
	{
		bodies[0] = b0;
		bodies[1] = b1;
	}

	void Contact::calculateInternals(float dt)
	{
		if(!bodies[0]) swapBodies();

		// Calculate contact basis
		calculateContactBasis();

		// Calculate the relative position
		relativeContactPosition[0] = contactPoint - bodies[0]->getPosition();
		if(bodies[1])
			relativeContactPosition[1] = contactPoint - bodies[1]->getPosition();

		// Calculate relative velocity of the bodies at the contact point
		contactVelocity = calculateLocalVelocity(0, dt);
		if(bodies[1])
			contactVelocity -= calculateLocalVelocity(1, dt);

		// Calculate desired change in velocity for resolution
		calculateDesiredDeltaVelocity(dt);
	}

	void Contact::swapBodies()
	{
		// Need to reverse the contact normal when swapping the bodies
		contactNormal *= -1;
		std::swap(bodies[0], bodies[1]);
	}

	void Contact::calculateContactBasis()
	{
		vec3 contactTangent[2];

		// Check whether the Z-axis is nearer to the X or Y axis
		if(abs(contactNormal.x) > abs(contactNormal.y))
		{
			// Scaling factor to ensure the results are normalised
			const float s = 1.0f/sqrt(contactNormal.z*contactNormal.z + contactNormal.x*contactNormal.x);

			// The new X-axis is at right angles to the world Y-axis
			contactTangent[0].x = contactNormal.z*s;
			contactTangent[0].y = 0;
			contactTangent[0].z = -contactNormal.x*s;

			// The new Y-axis is at right angles to the new X- and Z- axes
			contactTangent[1].x = contactNormal.y*contactTangent[0].x;
			contactTangent[1].y = contactNormal.z*contactTangent[0].x - contactNormal.x*contactTangent[0].z;
			contactTangent[1].z = -contactNormal.y*contactTangent[0].x;
		}
		else
		{
			// Scaling factor to ensure the results are normalised
			const float s = 1.0f/sqrt(contactNormal.z*contactNormal.z + contactNormal.y*contactNormal.y);

			// The new X-axis is at right angles to the world X-axis
			contactTangent[0].x = 0;
			contactTangent[0].y = -contactNormal.z*s;
			contactTangent[0].z = contactNormal.y*s;

			// The new Y-axis is at right angles to the new X- and Z- axes
			contactTangent[1].x = contactNormal.y*contactTangent[0].z - contactNormal.z*contactTangent[0].y;
			contactTangent[1].y = -contactNormal.x*contactTangent[0].z;
			contactTangent[1].z = contactNormal.x*contactTangent[0].y;
		}

		// Make a matrix from the three vectors.
		contactToWorld.setComponents(
			contactNormal,
			contactTangent[0],
			contactTangent[1]);
	}

	vec3 Contact::calculateLocalVelocity(int bodyIndex, float dt)
	{
		Body* thisBody = bodies[bodyIndex];

		// Calculate the velocity at contact
		vec3 velocity = cross(thisBody->getRotation(), relativeContactPosition[bodyIndex]);// From point angular velocity
		velocity += thisBody->getVelocity();// From object linear velocity

		// Turn the velocity into contact coordinates
		vec3 contactVelocity = contactToWorld.transformTranspose(velocity);

		// Calculate the amount of velocity that is due to forces without reactions
		//vec3 accVelocity = thisBody->getLastFrameAcceleration*dt;

		//// Calculate the velocity in contact-coordinates.
		//accVelocity = contactToWorld.transformTranspose(accVelocity);

		// TODO FRICTION
		//// We ignore any component of acceleration in the contact normal
		//// direction, we are only interested in planar acceleration
		//accVelocity.x = 0;

		//// Add the planar velocities - if there's enough friction they will
		//// be removed during velocity resolution
		//contactVelocity += accVelocity;

		return contactVelocity;
	}

	void Contact::calculateDesiredDeltaVelocity(float dt)
	{
    	const static float velocityLimit = 0.01f;

		float velocityFromAcc = dot(bodies[0]->getLastFrameAcceleration(), contactNormal*dt);
		if(bodies[1])
			velocityFromAcc -= dot(bodies[1]->getLastFrameAcceleration(), contactNormal*dt);

		// If the velocity is very slow, limit the restitution
		float thisRestitution = restitution;

		if(abs(contactVelocity.x) < velocityLimit)
		{
			thisRestitution = 0.0f;
		}

		// Combine the bounce velocity with the removed acceleration velocity
		desiredDeltaVelocity = -contactVelocity.x - thisRestitution* (contactVelocity.x - velocityFromAcc);
	}

	void Contact::applyPositionChange(vec3 linearChange[2], vec3 angularChange[2], float penetration)
	{
		const float angularLimit = 0.2f;
		float angularMove[2];
		float linearMove[2];

		float totalInertia = 0;
		float linearInertia[2];
		float angularInertia[2];

		//---------- Calculate inertia ----------//
		// We need to work out the inertia of each object in the direction
		// of the contact normal, due to angular inertia only.
		for (unsigned i = 0; i < 2; i++) if(bodies[i])
		{
			mat3 inverseInertiaTensor = bodies[i]->getInverseInertiaTensorWorld();

			vec3 angularInertiaWorld = cross(relativeContactPosition[i], contactNormal);
			angularInertiaWorld = inverseInertiaTensor.transform(angularInertiaWorld);
			angularInertiaWorld = cross(angularInertiaWorld, relativeContactPosition[i]);
			angularInertia[i] = dot(angularInertiaWorld, contactNormal);

			// The linear component is simply the inverse mass
			linearInertia[i] = bodies[i]->getInverseMass();

			// Keep track of the total inertia from all components
			totalInertia += linearInertia[i] + angularInertia[i];

			// We break the loop here so that the totalInertia value is
			// completely calculated (by both iterations) before
			// continuing.
		}

		//---------- Apply changes ----------//
		// Loop through again calculating and applying the changes
		for(unsigned i = 0; i < 2; i++) if(bodies[i])
		{
			// The linear and angular movements required are in proportion to the two inverse inertias
			float sign = (i == 0)?1:-1;
			angularMove[i] = sign * penetration * (angularInertia[i] / totalInertia);
			linearMove[i]  = sign * penetration * (linearInertia[i]  / totalInertia);

			// To avoid angular projections that are too great (when mass is large but inertia tensor is small) limit the angular move
			vec3 projection = relativeContactPosition[i] + contactNormal*dot(-relativeContactPosition[i], contactNormal);

			// Use the small angle approximation for the sine of the angle (i.e.
			// the magnitude would be sine(angularLimit) * projection.magnitude
			// but we approximate sine(angularLimit) to angularLimit).
			float maxMagnitude = angularLimit * projection.length();

			if(angularMove[i] < -maxMagnitude)
			{
				float totalMove = angularMove[i] + linearMove[i];
				angularMove[i] = -maxMagnitude;
				linearMove[i] = totalMove - angularMove[i];
			}
			else if(angularMove[i] > maxMagnitude)
			{
				float totalMove = angularMove[i] + linearMove[i];
				angularMove[i] = maxMagnitude;
				linearMove[i] = totalMove - angularMove[i];
			}

			// We have the linear amount of movement required by turning
			// the rigid body (in angularMove[i]). We now need to
			// calculate the desired rotation to achieve that.
			if(angularMove[i] == 0)
			{
				// Easy case - no angular movement means no rotation.
				angularChange[i].clear();
			}
			else
			{
				// Work out the direction we'd like to rotate in.
				vec3 targetAngularDirection = cross(relativeContactPosition[i], contactNormal);

				mat3 inverseInertiaTensor = bodies[i]->getInverseInertiaTensorWorld();

				// Work out the direction we'd need to rotate to achieve that
				angularChange[i] = inverseInertiaTensor.transform(targetAngularDirection) * (angularMove[i] / angularInertia[i]);
			}

			// Velocity change is easier - it is just the linear movement
			// along the contact normal.
			linearChange[i] = contactNormal * linearMove[i];

			// Now we can start to apply the values we've calculated.
			// Apply the linear movement
			vec3 pos = bodies[i]->getPosition();
			pos += contactNormal*linearMove[i];
			bodies[i]->setPosition(pos);

			// And the change in orientation
			quat q = bodies[i]->getOrientation();
			q += angularChange[i];
			bodies[i]->setOrientation(q);

			// We need to calculate the derived data for any body that is
			// asleep, so that the changes are reflected in the object's
			// data. Otherwise the resolution will not change the position
			// of the object, and the next collision detection round will
			// have the same penetration.
			//if (!body[i]->getAwake()) body[i]->calculateDerivedData();
		}
	}

	void Contact::applyVelocityChange(vec3 velocityChange[2], vec3 rotationChange[2])
	{
		// Get hold of the inverse mass and inverse inertia tensor, both in
		// world coordinates.
		mat3 inverseInertiaTensor[2] = {
			bodies[0]->getInverseInertiaTensorWorld(),
			bodies[1]->getInverseInertiaTensorWorld()
		};

		// We will calculate the impulse for each contact axis
		vec3 impulseContact;

		//if(friction == (real)0.0)
		//{
		//	// Use the short format for frictionless contacts
		//	impulseContact = calculateFrictionlessImpulse(inverseInertiaTensor);
		//}
		//else
		//{
		//	// Otherwise we may have impulses that aren't in the direction of the
		//	// contact, so we need the more complex version.
		//	impulseContact = calculateFrictionImpulse(inverseInertiaTensor);
		//}

		// Convert impulse to world coordinates
		vec3 impulse = contactToWorld.transform(impulseContact);

		// Split in the impulse into linear and rotational components
		vec3 impulsiveTorque = cross(relativeContactPosition[0], impulse);
		rotationChange[0] = inverseInertiaTensor[0].transform(impulsiveTorque);
		velocityChange[0].clear();
		velocityChange[0]+= impulse*bodies[0]->getInverseMass();

		// Apply the changes
		bodies[0]->addVelocity(velocityChange[0]);
		bodies[0]->addRotation(rotationChange[0]);

		if(bodies[1])
		{
			// Work out body one's linear and angular changes
			vec3 impulsiveTorque = cross(impulse, relativeContactPosition[1]);
			rotationChange[1] = inverseInertiaTensor[1].transform(impulsiveTorque);
			velocityChange[1].clear();
			velocityChange[1] += impulse*(-bodies[1]->getInverseMass());

			// And apply them.
			bodies[1]->addVelocity(velocityChange[1]);
			bodies[1]->addRotation(rotationChange[1]);
		}
	}
}
