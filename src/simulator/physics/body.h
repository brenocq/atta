//--------------------------------------------------
// Atta Physics
// body.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_BODY_H
#define ATTA_PHY_BODY_H

#include "simulator/math/math.h"

namespace atta::phy
{
	class Body
	{
		public:
			Body(vec3 position={0,0,0}, vec3 rotation={0,0,0}, float mass=1.0);
			~Body();

			void addForce(vec3 force);
			void addForceAtBodyPoint(vec3 force, vec3 point);
			void addForceAtPoint(vec3 force, vec3 point);

			void integrate(float dt);

			//---------- Helpers ----------//
			vec3 getPointInWorldSpace(vec3 point);

			//---------- Getters ----------//
			vec3 getPosition() const { return _position; };
			vec3 getVelocity() const { return _velocity; };
			vec3 getAcceleration() const { return _acceleration; };
			float getInverseMass() const { return _inverseMass; }
			float getMass() const { return _inverseMass<=0 ? 0 : _inverseMass; };
			float getDamping() const { return _damping; };

			//---------- Setters ----------//
			void setPosition(vec3 position) { _position = position; };
			void setVelocity(vec3 velocity) { _velocity = velocity; };
			void setAcceleration(vec3 acceleration) { _acceleration = acceleration; };
			void setMass(float mass) { _inverseMass = mass>0 ? 1/mass : 0; };

		private:
			// Clear the forces
			void clearAccumulators();
			void calculateDerivedData();
			void calculateTransformMatrix();
			void transformInertiaTensor();

			// Linear
			float _inverseMass;
			float _damping;
			vec3 _position;
			vec3 _velocity;
			vec3 _acceleration;

			// Angular
			mat3 _inverseInertiaTensor;
			float _angularDamping;
			quat _orientation;
			vec3 _rotation;// Angular velocity
			
			// Accumulators
			vec3 _forceAccum;
			vec3 _torqueAccum;

			// Physics optimizations
			bool _isAwake;

			// Useful while rendering and some calculations
			mat4 _transformMatrix;
	};
}
#endif// ATTA_PHY_BODY_H
