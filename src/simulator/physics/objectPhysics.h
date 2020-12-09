//--------------------------------------------------
// Robot Simulator
// objectPhysics.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef OBJECT_PHYSICS_H
#define OBJECT_PHYSICS_H

#include "simulator/math/math.h"

class ObjectPhysics
{
	public:
		ObjectPhysics(atta::vec3 position={0,0,0}, atta::vec3 rotation={0,0,0}, float mass=1.0);
		~ObjectPhysics();

		void addForce(atta::vec3 force);
		void addForceAtBodyPoint(atta::vec3 force, atta::vec3 point);
		void addForceAtPoint(atta::vec3 force, atta::vec3 point);

		void integrate(float dt);

		//---------- Helpers ----------//
		atta::vec3 getPointInWorldSpace(atta::vec3 point);

		//---------- Getters ----------//
		atta::vec3 getPosition() const { return _position; };
		atta::vec3 getVelocity() const { return _velocity; };
		atta::vec3 getAcceleration() const { return _acceleration; };
		float getInverseMass() const { return _inverseMass; }
		float getMass() const { return _inverseMass<=0 ? 0 : _inverseMass; };
		float getDamping() const { return _damping; };

		//---------- Setters ----------//
		void setPosition(atta::vec3 position) { _position = position; };
		void setVelocity(atta::vec3 velocity) { _velocity = velocity; };
		void setAcceleration(atta::vec3 acceleration) { _acceleration = acceleration; };
		void setMass(float mass) { _inverseMass = mass>0 ? 1/mass : 0; };

	private:
		// Clear the forces
		void clearAccumulators();
		void calculateDerivedData();
		void calculateTransformMatrix();
		void transformInertiaTensor();

		bool _isAwake;

		// Linear
		float _inverseMass;
		float _damping;
		atta::vec3 _position;
		atta::vec3 _velocity;
		atta::vec3 _acceleration;

		// Angular
		atta::mat3 _inverseInertiaTensor;
		float _angularDamping;
		atta::quat _orientation;
		atta::vec3 _rotation;// Angular velocity
		
		// Accumulators
		atta::vec3 _forceAccum;
		atta::vec3 _torqueAccum;

		// Useful while rendering and some calculations
		atta::mat4 _transformMatrix;
};

#endif// OBJECT_PHYSICS_H
