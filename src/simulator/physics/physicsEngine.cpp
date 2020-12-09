//--------------------------------------------------
// Robot PhysicsEngine
// physicsEngine.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "physicsEngine.h"
#include "forces/forces.h"

PhysicsEngine::PhysicsEngine()
{
	_forceGenerator = new ForceGenerator();
}

PhysicsEngine::~PhysicsEngine()
{
	if(_forceGenerator != nullptr)
	{
		delete _forceGenerator;
		_forceGenerator = nullptr;
	}
}

void PhysicsEngine::stepPhysics(float dt)
{
	dt/=1.f;

	_forceGenerator->updateForces(dt);
	for(auto object : _objectsPhysics)
	{
		//object->addForce({0,-9.8,0});
		object->integrate(dt);
	}
}

void PhysicsEngine::addObjectPhysics(ObjectPhysics* objectPhysics)
{
	_objectsPhysics.push_back(objectPhysics);
	_forceGenerator->add(_objectsPhysics.back(), new AnchoredSpringForce({0,3,0}, 0.1, 1));
}

bool PhysicsEngine::raycast(atta::vec3 startPosition, atta::vec3 direction)
{
	return false;
}

//---------- Static functions ----------//
atta::vec3 PhysicsEngine::getMouseClickRay(int x, int y, int width, int height, atta::vec3 camPos, atta::vec3 camForward, atta::vec3 camUp)
{
	//const float nearPlane = 0.1f;
	const float farPlane = 1000.0f;

	// Calculate fielf-of-view
	float tanFov = 1.0f;//1.0f/nearPlane;
	//float fov = 2.0*atan(tanFov);

	// Get ray pointing forward from the camera and extend it to the far plane
	atta::vec3 rayForward = camForward.unit();
	rayForward *= farPlane;

	// Find horizontal and vertical vectors relative to the camera
	atta::vec3 ver = {0.0f, 1.0f, 0.0f};
	atta::vec3 hor = rayForward.cross(ver);
	hor.normalize();
	ver = hor.cross(rayForward);
	ver.normalize();
	hor *= 2.f * farPlane * tanFov;
	ver *= 2.f * farPlane * tanFov;

	// Calculate aspect ratio
	float aspect = width/(float)height;

	// Adjust forward-ray based on the X/Y coordinates that were clicked
	hor *= aspect;
	atta::vec3 rayToCenter = camPos + rayForward;
	atta::vec3 dHor = hor * 1.f/(float)width;
	atta::vec3 dVer = ver * 1.f/(float)height;
	atta::vec3 rayTo = rayToCenter - hor*0.5f + ver*0.5f;
	rayTo += dHor*float(x);
	rayTo -= dVer*float(y);
		
	return rayTo;
}
