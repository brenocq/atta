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
		void addForceAtBodyPoint(glm::vec3 force, glm::vec3 point);
		void addForceAtPoint(glm::vec3 force, glm::vec3 point);

		void integrate(float dt);

		//---------- Helpers ----------//
		glm::vec3 getPointInWorldSpace(glm::vec3 point);

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
		void clearAccumulators();
		void calculateDerivedData();

		bool _isAwake;

		// Linear
		float _inverseMass;
		float _damping;
		glm::vec3 _position;
		glm::vec3 _velocity;
		glm::vec3 _acceleration;

		// Angular
		glm::mat3 _inverseInertiaTensor;
		float _angularDamping;
		glm::quat _orientation;
		glm::vec3 _rotation;// Angular velocity
		
		// Accumulators
		glm::vec3 _forceAccum;
		glm::vec3 _torqueAccum;

		// Useful while rendering and some calculations
		glm::mat4 _transformMatrix;
};

#endif// OBJECT_PHYSICS_H
