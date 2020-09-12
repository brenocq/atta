//--------------------------------------------------
// Robot Simulator
// objectPhysics.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef OBJECT_PHYSICS_H
#define OBJECT_PHYSICS_H

#include "glm.h"

class ObjectPhysics
{
	public:
		ObjectPhysics(glm::vec3 position={0,0,0}, glm::vec3 rotation={0,0,0}, float mass=1.0);
		~ObjectPhysics();

		void addForce(glm::vec3 force);
		void integrate(float dt);

		//---------- Getters ----------//
		glm::vec3 getPosition() const { return _position; };
		glm::vec3 getVelocity() const { return _velocity; };
		glm::vec3 getAcceleration() const { return _acceleration; };
		float getInverseMass() const { return _inverseMass; }
		float getMass() const { return _inverseMass<=0 ? 0 : _inverseMass; };
		float getDamping() const { return _damping; };

		//---------- Setters ----------//
		void setPosition(glm::vec3 position) { _position = position; };
		void setVelocity(glm::vec3 velocity) { _velocity = velocity; };
		void setAcceleration(glm::vec3 acceleration) { _acceleration = acceleration; };
		void setMass(float mass) { _inverseMass = mass>0 ? 1/mass : 0; };

	private:
		// Clear the forces
		void clearAccumulator();

		glm::vec3 _position;
		glm::vec3 _velocity;
		glm::vec3 _acceleration;
		
		glm::vec3 _forceAccum;

		// Damping is required to remove energy added 
		// through numerical instability in the integrator
		float _damping;

		// Inverse mass is more useful because integration is simpler
		// Immovable objects have zero inverseMass (infinity mass)
		float _inverseMass;
};

#endif// OBJECT_PHYSICS_H
