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

		//----- Calculate linear velocity (world) at point of contact -----//
		vec3 velocity;
		// From point angular velocity
		velocity += cross(thisBody->getRotation(), relativeContactPosition[bodyIndex]);
		// From object linear velocity
		velocity += thisBody->getVelocity();

		//----- Transform from world coordinates to contact coordinates -----//
		vec3 contactVelocity = contactToWorld.transformTranspose(velocity);

		//----- Add planar velocities from last acc -----//
		// Calculate the amount of velocity that is due to forces without reactions
		vec3 accVelocity = thisBody->getLastFrameAcceleration()*dt;

		// Calculate the velocity in contact-coordinates.
		accVelocity = contactToWorld.transformTranspose(accVelocity);

		// We ignore any component of acceleration in the contact normal
		// direction, we are only interested in planar acceleration
		accVelocity.x = 0;

		// Add the planar velocities - if there's enough friction they will
		// be removed during velocity resolution
		contactVelocity += accVelocity;

		return contactVelocity;
	}

	void Contact::calculateDesiredDeltaVelocity(float dt)
	{
    	const static float velocityLimit = 0.01f;

		float velocityFromAcc = 0;
		if(bodies[0]->getIsAwake())
			velocityFromAcc += dot(bodies[0]->getLastFrameAcceleration()*dt, contactNormal);

		if(bodies[1] && bodies[1]->getIsAwake())
			velocityFromAcc -= dot(bodies[1]->getLastFrameAcceleration()*dt, contactNormal);

		// If the velocity is very slow, limit the restitution
		float thisRestitution = restitution;

		if(abs(contactVelocity.x) < velocityLimit)
		{
			thisRestitution = 0.0f;
		}

		// Combine the bounce velocity with the removed acceleration velocity
		desiredDeltaVelocity = 
			-contactVelocity.x 
			-thisRestitution * (contactVelocity.x - velocityFromAcc);
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

			Log::debug("Contact", "ContactPos:$0", contactPoint.toString());
			Log::debug("Contact", "RelContactPos:$0", relativeContactPosition[i].toString());
			vec3 angularInertiaWorld = cross(relativeContactPosition[i], contactNormal);
			Log::debug("Contact", "cross:$0", angularInertiaWorld.toString());
			angularInertiaWorld = inverseInertiaTensor.transform(angularInertiaWorld);
			Log::debug("Contact", "inerciaTensor:$0", inverseInertiaTensor.toString());
			Log::debug("Contact", "transform:$0", angularInertiaWorld.toString());
			angularInertiaWorld = cross(angularInertiaWorld, relativeContactPosition[i]);
			Log::debug("Contact", "crossAgain:$0", angularInertiaWorld.toString());
			angularInertia[i] = dot(angularInertiaWorld, contactNormal);
			Log::debug("Contact", "angIn:$0", angularInertia[i]);
			Log::debug("Contact", "------------");
			//[Contact] ContactPos:vec3{0.500000, 0.000000, 0.500000} 
			//[Contact] RelContactPos:vec3{0.500000, -0.498905, 0.500000} 
			//[Contact] cross:vec3{-0.500000, 0.000000, 0.500000} 
			//[Contact] transform:vec3{-11.999999, 0.000000, 11.999999} 
			//[Contact] crossAgain:vec3{5.986863, 11.999999, 5.986863} 
			//[Contact] angIn:11.999999 


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
				//Log::debug("Contact", "($0)-> AngMove:$1 AngChg:$2 AngIn:$3", i, angularMove[i], angularChange[i].toString(), angularInertia[i]);
			}

			// Velocity change is easier - it is just the linear movement
			// along the contact normal.
			linearChange[i] = contactNormal*linearMove[i];

			// Now we can start to apply the values we've calculated.
			// Apply the linear movement
			vec3 pos = bodies[i]->getPosition();
			pos += linearChange[i];
			bodies[i]->setPosition(pos);

			// And the change in orientation
			quat q = bodies[i]->getOrientation();
			//Log::debug("Contact", "Ori:$0", q.toString());
			q += angularChange[i];
			bodies[i]->setOrientation(q);

			// We need to calculate the derived data for any body that is
			// asleep, so that the changes are reflected in the object's
			// data. Otherwise the resolution will not change the position
			// of the object, and the next collision detection round will
			// have the same penetration.
			if(!bodies[i]->getIsAwake()) bodies[i]->calculateDerivedData();
		}
	}

	void Contact::applyVelocityChange(vec3 velocityChange[2], vec3 rotationChange[2])
	{
		// Get hold of the inverse mass and inverse inertia tensor, both in
		// world coordinates.
		mat3 inverseInertiaTensor[2];
		inverseInertiaTensor[0] = bodies[0]->getInverseInertiaTensorWorld();
		if(bodies[1])
			inverseInertiaTensor[1] = bodies[1]->getInverseInertiaTensorWorld();

		// We will calculate the impulse for each contact axis
		vec3 impulseContact;

		if(friction == 0.0f)
		{
			// Use the short format for frictionless contacts
			impulseContact = calculateFrictionlessImpulse(inverseInertiaTensor);
		}
		else
		{
			// Otherwise we may have impulses that aren't in the direction of the
			// contact, so we need the more complex version
			impulseContact = calculateFrictionImpulse(inverseInertiaTensor);
		}

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

	void Contact::matchAwakeState()
	{
		// Collisions with the world never cause a body to wake up.
		if (!bodies[1]) return;

		bool body0awake = bodies[0]->getIsAwake();
		bool body1awake = bodies[1]->getIsAwake();

		// Wake up only the sleeping one
		if (body0awake ^ body1awake) {
			if (body0awake) bodies[1]->setIsAwake();
			else bodies[0]->setIsAwake();
		}
	}

	vec3 Contact::calculateFrictionlessImpulse(mat3 *inverseInertiaTensor)
	{
		vec3 impulseContact;

		// Build a vector that shows the change in velocity in
		// world space for a unit impulse in the direction of the contact
		// normal.
		vec3 deltaVelWorld = cross(relativeContactPosition[0], contactNormal);
		deltaVelWorld = inverseInertiaTensor[0].transform(deltaVelWorld);
		deltaVelWorld = cross(deltaVelWorld, relativeContactPosition[0]);

		// Work out the change in velocity in contact coordiantes.
		float deltaVelocity = dot(deltaVelWorld, contactNormal);

		// Add the linear component of velocity change
		deltaVelocity += bodies[0]->getInverseMass();

		// Check if we need to the second body's data
		if(bodies[1])
		{
			// Go through the same transformation sequence again
			vec3 deltaVelWorld = cross(relativeContactPosition[1], contactNormal);
			deltaVelWorld = inverseInertiaTensor[1].transform(deltaVelWorld);
			deltaVelWorld = cross(deltaVelWorld, relativeContactPosition[1]);

			// Add the change in velocity due to rotation
			deltaVelocity += dot(deltaVelWorld, contactNormal);

			// Add the change in velocity due to linear motion
			deltaVelocity += bodies[1]->getInverseMass();
		}

		// Calculate the required size of the impulse
		impulseContact.x = desiredDeltaVelocity / deltaVelocity;
		impulseContact.y = 0;
		impulseContact.z = 0;
		return impulseContact;
	}

	vec3 Contact::calculateFrictionImpulse(mat3 *inverseInertiaTensor)
	{
		vec3 impulseContact;
		float inverseMass = bodies[0]->getInverseMass();

		// The equivalent of a cross product in matrices is multiplication
		// by a skew symmetric matrix - we build the matrix for converting
		// between linear and angular quantities
		mat3 impulseToTorque;
		impulseToTorque.setSkewSymmetric(relativeContactPosition[0]);

		// Build the matrix to convert contact impulse to change in velocity
		// in world coordinates
		mat3 deltaVelWorld = impulseToTorque;
		deltaVelWorld *= inverseInertiaTensor[0];
		deltaVelWorld *= impulseToTorque;
		deltaVelWorld *= -1;

		// Check if we need to add body two's data
		if(bodies[1])
		{
			// Set the cross product matrix
			impulseToTorque.setSkewSymmetric(relativeContactPosition[1]);

			// Calculate the velocity change matrix
			mat3 deltaVelWorld2 = impulseToTorque;
			deltaVelWorld2 *= inverseInertiaTensor[1];
			deltaVelWorld2 *= impulseToTorque;
			deltaVelWorld2 *= -1;

			// Add to the total delta velocity.
			deltaVelWorld += deltaVelWorld2;

			// Add to the inverse mass
			inverseMass += bodies[1]->getInverseMass();
		}

		// Do a change of basis to convert into contact coordinates.
		mat3 deltaVelocity = transpose(contactToWorld);
		deltaVelocity *= deltaVelWorld;
		deltaVelocity *= contactToWorld;

		// Add in the linear velocity change
		deltaVelocity.data[0] += inverseMass;
		deltaVelocity.data[4] += inverseMass;
		deltaVelocity.data[8] += inverseMass;

		// Invert to get the impulse needed per unit velocity
		mat3 impulseMatrix = inverse(deltaVelocity);

		// Find the target velocities to kill
		vec3 velKill(desiredDeltaVelocity,
			-contactVelocity.y,
			-contactVelocity.z);

		// Find the impulse to kill target velocities
		impulseContact = impulseMatrix.transform(velKill);

		// Check for exceeding friction
		float planarImpulse = sqrt(
			impulseContact.y*impulseContact.y +
			impulseContact.z*impulseContact.z
			);
		if(planarImpulse > impulseContact.x * friction)
		{
			// We need to use dynamic friction
			impulseContact.y /= planarImpulse;
			impulseContact.z /= planarImpulse;

			impulseContact.x = deltaVelocity.data[0] +
				deltaVelocity.data[1]*friction*impulseContact.y +
				deltaVelocity.data[2]*friction*impulseContact.z;
			impulseContact.x = desiredDeltaVelocity / impulseContact.x;
			impulseContact.y *= friction * impulseContact.x;
			impulseContact.z *= friction * impulseContact.x;
		}
		return impulseContact;
	}
}
