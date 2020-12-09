//--------------------------------------------------
// Robot Simulator
// objectPhysics.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "objectPhysics.h"

ObjectPhysics::ObjectPhysics(atta::vec3 position, atta::vec3 rotation, float mass):
	_position(position)
{
	if(mass > 0)
		_inverseMass = 1/mass;
	else
		_inverseMass = 0;

	_damping = 0.99;
	_velocity = atta::vec3(0,0,0);
	_acceleration = atta::vec3(0,0,0);


	_orientation = atta::quat();
	// TODO initialize inertia tensor from shape

	_forceAccum = atta::vec3(0,0,0);
}

ObjectPhysics::~ObjectPhysics()
{
}

void ObjectPhysics::addForce(atta::vec3 force)
{
	_forceAccum += force;
	_isAwake = true;
}

void ObjectPhysics::addForceAtBodyPoint(atta::vec3 force, atta::vec3 point)
{
	atta::vec3 worldPoint = getPointInWorldSpace(point);
	addForceAtPoint(force, worldPoint);
}

void ObjectPhysics::addForceAtPoint(atta::vec3 force, atta::vec3 point)
{
	// Convert point to relative to center of mass
	atta::vec3 pt = point - _position;

	_forceAccum += force;
	_torqueAccum += pt.cross(force);

	_isAwake = true;
}

void ObjectPhysics::integrate(float dt)
{
	if(_inverseMass<=0)
		return;

	// Calculate linear acceleration
	atta::vec3 resultingAcc = _acceleration;
	resultingAcc += _forceAccum*_inverseMass;

	// Calculate angular acceleration
	atta::vec3 resultingAngAcc = _inverseInertiaTensor*_torqueAccum;;

	// Update linear velocity
	_velocity += resultingAcc*dt;
	_velocity *= powf(_damping, dt);

	// Update angular velocity
	_rotation += resultingAngAcc*dt;
	_rotation *= powf(_angularDamping, dt);

	// Update linear position
	_position += _velocity*dt;

	// TODO Update angular position
	//_orientation +=	_rotation*dt; 

	calculateDerivedData();
	clearAccumulators();
}

//------------------------------//
//----------- HELPERS ----------//
//------------------------------//
atta::vec3 ObjectPhysics::getPointInWorldSpace(atta::vec3 point)
{
	return atta::vec3(_transformMatrix*point);
}

//------------------------------//
//----------- PRIVATE ----------//
//------------------------------//

void ObjectPhysics::clearAccumulators()
{
	_forceAccum = atta::vec3(0,0,0);
	_torqueAccum = atta::vec3(0,0,0);
}

void ObjectPhysics::calculateDerivedData()
{
	_orientation.normalize();

	// Calculate transform matrix
	calculateTransformMatrix();

	// TODO Calculate inertia tensor in world space 
	transformInertiaTensor();
}

void ObjectPhysics::calculateTransformMatrix()
{
    _transformMatrix.data[0] = 1-2*_orientation.j*_orientation.j-
        2*_orientation.k*_orientation.k;
    _transformMatrix.data[1] = 2*_orientation.i*_orientation.j -
        2*_orientation.r*_orientation.k;
    _transformMatrix.data[2] = 2*_orientation.i*_orientation.k +
        2*_orientation.r*_orientation.j;
    _transformMatrix.data[3] = _position.x;

    _transformMatrix.data[4] = 2*_orientation.i*_orientation.j +
        2*_orientation.r*_orientation.k;
    _transformMatrix.data[5] = 1-2*_orientation.i*_orientation.i-
        2*_orientation.k*_orientation.k;
    _transformMatrix.data[6] = 2*_orientation.j*_orientation.k -
        2*_orientation.r*_orientation.i;
    _transformMatrix.data[7] = _position.y;

    _transformMatrix.data[8] = 2*_orientation.i*_orientation.k -
        2*_orientation.r*_orientation.j;
    _transformMatrix.data[9] = 2*_orientation.j*_orientation.k +
        2*_orientation.r*_orientation.i;
    _transformMatrix.data[10] = 1-2*_orientation.i*_orientation.i-
        2*_orientation.j*_orientation.j;
    _transformMatrix.data[11] = _position.z;
}

void ObjectPhysics::transformInertiaTensor()
{
    //float t4 = _transformMatrix.data[0]*iitBody.data[0]+
    //    _transformMatrix.data[1]*iitBody.data[3]+
    //    _transformMatrix.data[2]*iitBody.data[6];
    //float t9 = _transformMatrix.data[0]*iitBody.data[1]+
    //    _transformMatrix.data[1]*iitBody.data[4]+
    //    _transformMatrix.data[2]*iitBody.data[7];
    //float t14 = _transformMatrix.data[0]*iitBody.data[2]+
    //    _transformMatrix.data[1]*iitBody.data[5]+
    //    _transformMatrix.data[2]*iitBody.data[8];
    //float t28 = _transformMatrix.data[4]*iitBody.data[0]+
    //    _transformMatrix.data[5]*iitBody.data[3]+
    //    _transformMatrix.data[6]*iitBody.data[6];
    //float t33 = _transformMatrix.data[4]*iitBody.data[1]+
    //    _transformMatrix.data[5]*iitBody.data[4]+
    //    _transformMatrix.data[6]*iitBody.data[7];
    //float t38 = _transformMatrix.data[4]*iitBody.data[2]+
    //    _transformMatrix.data[5]*iitBody.data[5]+
    //    _transformMatrix.data[6]*iitBody.data[8];
    //float t52 = _transformMatrix.data[8]*iitBody.data[0]+
    //    _transformMatrix.data[9]*iitBody.data[3]+
    //    _transformMatrix.data[10]*iitBody.data[6];
    //float t57 = _transformMatrix.data[8]*iitBody.data[1]+
    //    _transformMatrix.data[9]*iitBody.data[4]+
    //    _transformMatrix.data[10]*iitBody.data[7];
    //float t62 = _transformMatrix.data[8]*iitBody.data[2]+
    //    _transformMatrix.data[9]*iitBody.data[5]+
    //    _transformMatrix.data[10]*iitBody.data[8];

    //_inverseInertiaTensor.data[0] = t4*_transformMatrix.data[0]+
    //    t9*_transformMatrix.data[1]+
    //    t14*_transformMatrix.data[2];
    //_inverseInertiaTensor.data[1] = t4*_transformMatrix.data[4]+
    //    t9*_transformMatrix.data[5]+
    //    t14*_transformMatrix.data[6];
    //_inverseInertiaTensor.data[2] = t4*_transformMatrix.data[8]+
    //    t9*_transformMatrix.data[9]+
    //    t14*_transformMatrix.data[10];
    //_inverseInertiaTensor.data[3] = t28*_transformMatrix.data[0]+
    //    t33*_transformMatrix.data[1]+
    //    t38*_transformMatrix.data[2];
    //_inverseInertiaTensor.data[4] = t28*_transformMatrix.data[4]+
    //    t33*_transformMatrix.data[5]+
    //    t38*_transformMatrix.data[6];
    //_inverseInertiaTensor.data[5] = t28*_transformMatrix.data[8]+
    //    t33*_transformMatrix.data[9]+
    //    t38*_transformMatrix.data[10];
    //_inverseInertiaTensor.data[6] = t52*_transformMatrix.data[0]+
    //    t57*_transformMatrix.data[1]+
    //    t62*_transformMatrix.data[2];
    //_inverseInertiaTensor.data[7] = t52*_transformMatrix.data[4]+
    //    t57*_transformMatrix.data[5]+
    //    t62*_transformMatrix.data[6];
    //_inverseInertiaTensor.data[8] = t52*_transformMatrix.data[8]+
    //    t57*_transformMatrix.data[9]+
    //    t62*_transformMatrix.data[10];
}
