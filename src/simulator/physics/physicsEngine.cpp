//--------------------------------------------------
// Robot PhysicsEngine
// physicsEngine.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "physicsEngine.h"

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
	dt/=10.f;
	for(auto object : _objectsPhysics)
	{
		object->addForce({0,9.8,0});
		object->integrate(dt);
	}
}

void PhysicsEngine::addObjectPhysics(ObjectPhysics* objectPhysics)
{
	_objectsPhysics.push_back(objectPhysics);
}

bool PhysicsEngine::raycast(glm::vec3 startPosition, glm::vec3 direction)
{
	return false;
}

//---------- Static functions ----------//
glm::vec3 PhysicsEngine::getMouseClickRay(int x, int y, int width, int height, glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camUp)
{
	//const float nearPlane = 0.1f;
	const float farPlane = 1000.0f;

	// Calculate fielf-of-view
	float tanFov = 1.0f;//1.0f/nearPlane;
	//float fov = 2.0*atan(tanFov);

	// Get ray pointing forward from the camera and extend it to the far plane
	glm::vec3 rayForward = glm::normalize(camForward);
	rayForward *= farPlane;

	// Find horizontal and vertical vectors relative to the camera
	glm::vec3 ver = {0.0f, 1.0f, 0.0f};
	glm::vec3 hor = glm::cross(rayForward, ver);
	hor = glm::normalize(hor);
	ver = glm::cross(hor, rayForward);
	ver = glm::normalize(ver);
	hor *= 2.f * farPlane * tanFov;
	ver *= 2.f * farPlane * tanFov;

	// Calculate aspect ratio
	float aspect = width/(float)height;

	// Adjust forward-ray based on the X/Y coordinates that were clicked
	hor *= aspect;
	glm::vec3 rayToCenter = camPos + rayForward;
	glm::vec3 dHor = hor * 1.f/(float)width;
	glm::vec3 dVer = ver * 1.f/(float)height;
	glm::vec3 rayTo = rayToCenter - hor*0.5f + ver*0.5f;
	rayTo += dHor*float(x);
	rayTo -= dVer*float(y);
		
	return rayTo;
}
