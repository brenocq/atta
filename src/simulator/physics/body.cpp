//--------------------------------------------------
// Atta Physics
// body.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "body.h"
#include "physicsConstants.h"

namespace atta::phy
{
	Body::Body(vec3* position, quat* orientation, float mass):
		_position(position), _orientation(orientation), 
		_isAwake(mass>0), _canSleep(true), _motion(mass>0?2*phy::sleepEpsilon:0)
	{
		if(mass > 0)
			_inverseMass = 1.0f/mass;
		else
			_inverseMass = 0;

		_damping = 0.95;
		_angularDamping = 0.8;
		_velocity = vec3(0,0,0);
		_acceleration = vec3(0,0,0);
		clearAccumulators();

		calculateDerivedData();
	}

	Body::~Body()
	{
	}

    void Body::setIsAwake(const bool awake)
	{
		if(awake)
		{
			_isAwake= true;
			// Add a bit of motion to avoid it falling asleep immediately
			_motion = phy::sleepEpsilon*2.0f;
		}
		else
		{
			_isAwake = false;
			_velocity.clear();
			_rotation.clear();
		}
	}

	void Body::addShape(std::shared_ptr<Shape> shape)
	{
		shape->setBody(this);
		_shapes.push_back(shape);

		// TODO calculating inverse inertia tensor only from last shape! (Should calculate based on all shapes)
		if(_inverseMass>0)
		{
			_inverseInertiaTensor = inverse(shape->calculateInertiaTensor(1.0/_inverseMass));
			transformInertiaTensor();
		}
	}

	void Body::addForce(vec3 force)
	{
		_forceAccum += force;
		_isAwake = true;
	}

	void Body::addForceAtBodyPoint(vec3 force, vec3 point)
	{
		vec3 worldPoint = getPointInWorldSpace(point);
		addForceAtPoint(force, worldPoint);
	}

	void Body::addForceAtPoint(vec3 force, vec3 point)
	{
		// Convert point to relative to center of mass
		vec3 pt = point - *_position;

		_forceAccum += force;
		_torqueAccum += pt.cross(force);

		_isAwake = true;
	}

	void Body::integrate(float dt)
	{
    	if(!_isAwake) return;

    	//----- Calculate accelerations -----//
    	// Calculate linear acceleration from force inputs.
		_lastFrameAcceleration = _acceleration;
    	_lastFrameAcceleration += _forceAccum*_inverseMass;

		// Calculate angular acceleration from torque inputs.
		vec3 angularAcceleration = _inverseInertiaTensorWorld.transform(_torqueAccum);

    	//----- Adjust velocities -----//
		// Update linear velocity
		_velocity += _lastFrameAcceleration*dt;
		_velocity *= powf(_damping, dt);

		// Update angular velocity
		_rotation += angularAcceleration*dt;
		_rotation *= powf(_angularDamping, dt);

    	//----- Adjust positions -----//
		// Update linear position
		*_position += _velocity*dt;

		// Update angular position
		*_orientation += _rotation*dt; 

		calculateDerivedData();
		clearAccumulators();

		// Update the kinetic energy store, and possibly put the body to
		// sleep.
		if(_canSleep)
		{
			float currentMotion = dot(_velocity, _velocity) + dot(_rotation, _rotation);

			float bias = powf(0.5, dt);
			_motion = bias*_motion + (1-bias)*currentMotion;

			if (_motion < phy::sleepEpsilon) setIsAwake(false);
			else if (_motion > 10 * phy::sleepEpsilon) _motion = 10 * phy::sleepEpsilon;
		}
	}

	//------------------------------//
	//----------- HELPERS ----------//
	//------------------------------//
	vec3 Body::getPointInWorldSpace(vec3 point)
	{
		return vec3(_transformMatrix*point);
	}

	//------------------------------//
	//----------- PRIVATE ----------//
	//------------------------------//

	void Body::clearAccumulators()
	{
		_forceAccum.clear();
		_torqueAccum.clear();
	}

	void Body::calculateDerivedData()
	{
		_orientation->normalize();

		// Calculate transform matrix
		calculateTransformMatrix();

		transformInertiaTensor();
	}

	void Body::calculateTransformMatrix()
	{
		_transformMatrix.data[0] = 1-2*(*_orientation).j*(*_orientation).j-
			2*(*_orientation).k*(*_orientation).k;
		_transformMatrix.data[1] = 2*(*_orientation).i*(*_orientation).j -
			2*(*_orientation).r*(*_orientation).k;
		_transformMatrix.data[2] = 2*(*_orientation).i*(*_orientation).k +
			2*(*_orientation).r*(*_orientation).j;
		_transformMatrix.data[3] = _position->x;

		_transformMatrix.data[4] = 2*(*_orientation).i*(*_orientation).j +
			2*(*_orientation).r*(*_orientation).k;
		_transformMatrix.data[5] = 1-2*(*_orientation).i*(*_orientation).i-
			2*(*_orientation).k*(*_orientation).k;
		_transformMatrix.data[6] = 2*(*_orientation).j*(*_orientation).k -
			2*(*_orientation).r*(*_orientation).i;
		_transformMatrix.data[7] = _position->y;

		_transformMatrix.data[8] = 2*(*_orientation).i*(*_orientation).k -
			2*(*_orientation).r*(*_orientation).j;
		_transformMatrix.data[9] = 2*(*_orientation).j*(*_orientation).k +
			2*(*_orientation).r*(*_orientation).i;
		_transformMatrix.data[10] = 1-2*(*_orientation).i*(*_orientation).i-
			2*(*_orientation).j*(*_orientation).j;
		_transformMatrix.data[11] = _position->z;
	}

	void Body::transformInertiaTensor()
	{
		// Calculate the inertia tensor in world coordinates
		float t4 = _transformMatrix.data[0]*_inverseInertiaTensor.data[0] + 
			_transformMatrix.data[1]*_inverseInertiaTensor.data[3] + 
			_transformMatrix.data[2]*_inverseInertiaTensor.data[6];
		float t9 = _transformMatrix.data[0]*_inverseInertiaTensor.data[1] + 
			_transformMatrix.data[1]*_inverseInertiaTensor.data[4] + 
			_transformMatrix.data[2]*_inverseInertiaTensor.data[7];
		float t14 = _transformMatrix.data[0]*_inverseInertiaTensor.data[2] + 
			_transformMatrix.data[1]*_inverseInertiaTensor.data[5] + 
			_transformMatrix.data[2]*_inverseInertiaTensor.data[8];
		float t28 = _transformMatrix.data[4]*_inverseInertiaTensor.data[0] + 
			_transformMatrix.data[5]*_inverseInertiaTensor.data[3] + 
			_transformMatrix.data[6]*_inverseInertiaTensor.data[6];
		float t33 = _transformMatrix.data[4]*_inverseInertiaTensor.data[1] + 
			_transformMatrix.data[5]*_inverseInertiaTensor.data[4] + 
			_transformMatrix.data[6]*_inverseInertiaTensor.data[7];
		float t38 = _transformMatrix.data[4]*_inverseInertiaTensor.data[2] + 
			_transformMatrix.data[5]*_inverseInertiaTensor.data[5] + 
			_transformMatrix.data[6]*_inverseInertiaTensor.data[8];
		float t52 = _transformMatrix.data[8]*_inverseInertiaTensor.data[0] + 
			_transformMatrix.data[9]*_inverseInertiaTensor.data[3] + 
			_transformMatrix.data[10]*_inverseInertiaTensor.data[6];
		float t57 = _transformMatrix.data[8]*_inverseInertiaTensor.data[1] + 
			_transformMatrix.data[9]*_inverseInertiaTensor.data[4] + 
			_transformMatrix.data[10]*_inverseInertiaTensor.data[7];
		float t62 = _transformMatrix.data[8]*_inverseInertiaTensor.data[2] + 
			_transformMatrix.data[9]*_inverseInertiaTensor.data[5] + 
			_transformMatrix.data[10]*_inverseInertiaTensor.data[8];

		_inverseInertiaTensorWorld.data[0] = t4*_transformMatrix.data[0] + t9*_transformMatrix.data[1] + t14*_transformMatrix.data[2];
		_inverseInertiaTensorWorld.data[1] = t4*_transformMatrix.data[4] + t9*_transformMatrix.data[5] + t14*_transformMatrix.data[6];
		_inverseInertiaTensorWorld.data[2] = t4*_transformMatrix.data[8] + t9*_transformMatrix.data[9] + t14*_transformMatrix.data[10];
		_inverseInertiaTensorWorld.data[3] = t28*_transformMatrix.data[0] + t33*_transformMatrix.data[1] + t38*_transformMatrix.data[2];
		_inverseInertiaTensorWorld.data[4] = t28*_transformMatrix.data[4] + t33*_transformMatrix.data[5] + t38*_transformMatrix.data[6];
		_inverseInertiaTensorWorld.data[5] = t28*_transformMatrix.data[8] + t33*_transformMatrix.data[9] + t38*_transformMatrix.data[10];
		_inverseInertiaTensorWorld.data[6] = t52*_transformMatrix.data[0] + t57*_transformMatrix.data[1] + t62*_transformMatrix.data[2];
		_inverseInertiaTensorWorld.data[7] = t52*_transformMatrix.data[4] + t57*_transformMatrix.data[5] + t62*_transformMatrix.data[6];
		_inverseInertiaTensorWorld.data[8] = t52*_transformMatrix.data[8] + t57*_transformMatrix.data[9] + t62*_transformMatrix.data[10];
		//mat3 localToWorld = inverse(mat3(_transformMatrix));
		//_inverseInertiaTensorWorld = transpose(localToWorld)*(_inverseInertiaTensor*localToWorld);
	}
}
