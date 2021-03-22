//--------------------------------------------------
// Atta Physics
// body.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_BODY_H
#define ATTA_PHYSICS_BODY_H

#include <vector>
#include <memory>
#include <atta/math/math.h>
#include <atta/physics/shapes/shapes.h>

namespace atta::phy
{
	class Body
	{
		friend class Contact;
		public:
			Body(vec3* position, quat* orientation, float mass=1.0);
			~Body();

			//---------- Shape ----------//
			void addShape(std::shared_ptr<Shape> shape);

			//---------- Force/Torque ----------//
			void addForce(vec3 force);
			void addForceAtBodyPoint(vec3 force, vec3 point);
			void addForceAtPoint(vec3 force, vec3 point);
			void integrate(float dt);

			//---------- Velocity ----------//
			void addVelocity(vec3 vel) { _velocity+=vel; }
			void addRotation(vec3 rot) { _rotation+=rot; }

			//---------- Helpers ----------//
			vec3 getPointInWorldSpace(vec3 point);

			//---------- Getters ----------//
			vec3 getPosition() const { return *_position; };
			vec3 getVelocity() const { return _velocity; };
			vec3 getAcceleration() const { return _acceleration; };
			vec3 getLastFrameAcceleration() const { return _lastFrameAcceleration; };
			float getInverseMass() const { return _inverseMass; }
			float getMass() const { return _inverseMass<=0 ? 0 : _inverseMass; };
			float getDamping() const { return _damping; };

			mat3 getInverseInertiaTensorWorld() const { return _inverseInertiaTensorWorld; }
			quat getOrientation() const { return *_orientation; };
			vec3 getRotation() const { return _rotation; };

			std::vector<std::shared_ptr<Shape>> getShapes() const { return _shapes; }
			mat4 getTransformMatrix() const { return _transformMatrix; };
			bool getIsAwake() const { return _isAwake; }

			//---------- Setters ----------//
			void setPosition(vec3 position) { *_position = position; };
			void setVelocity(vec3 velocity) { _velocity = velocity; };
			void setAcceleration(vec3 acceleration) { _acceleration = acceleration; };
			void setMass(float mass) { _inverseMass = mass>0 ? 1/mass : 0; };

			void setOrientation(quat orientation) { (*_orientation) = orientation; }
        	void setIsAwake(const bool awake=true);

		private:
			// Clear the forces
			void clearAccumulators();
			void calculateDerivedData();
			void calculateTransformMatrix();
			void transformInertiaTensor();

			// Shape
			std::vector<std::shared_ptr<Shape>> _shapes;

			// Linear
			float _inverseMass;
			float _damping;
			vec3* _position;
			vec3 _velocity;
			vec3 _acceleration;
			vec3 _lastFrameAcceleration;

			// Angular
			mat3 _inverseInertiaTensor;
			mat3 _inverseInertiaTensorWorld;
			float _angularDamping;
			quat* _orientation;
			vec3 _rotation;// Angular velocity
			
			// Accumulators
			vec3 _forceAccum;
			vec3 _torqueAccum;

			// Physics optimizations
			bool _isAwake;
			bool _canSleep;
			float _motion;// Stores the amount of kinect energy (if it is low, body will sleep)

			// Useful while rendering and some calculations
			mat4 _transformMatrix;
	};
}
#endif// ATTA_PHYSICS_BODY_H
