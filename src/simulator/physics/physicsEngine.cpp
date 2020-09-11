//--------------------------------------------------
// Robot PhysicsEngine
// physicsEngine.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "physicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
	// Create the collision configuration
	_bulletCollisionConfiguration = new btDefaultCollisionConfiguration();
	// Create the dispatcher
	_bulletDispatcher = new btCollisionDispatcher(_bulletCollisionConfiguration);
	// Create the broadphase
	_bulletBroadphase = new btDbvtBroadphase();
	// Create the constraint solver
	_bulletSolver = new btSequentialImpulseConstraintSolver;
	// Create the world
	_bulletWorld = new btDiscreteDynamicsWorld(_bulletDispatcher, _bulletBroadphase, _bulletSolver, _bulletCollisionConfiguration);

	_bulletWorld->setGravity(btVector3(0, -9.80665, 0));
}

PhysicsEngine::~PhysicsEngine()
{
	if(_bulletWorld != nullptr)
	{
		delete _bulletWorld;
		_bulletWorld = nullptr;
	}

	if(_bulletSolver != nullptr)
	{
		delete _bulletSolver;
		_bulletSolver = nullptr;
	}

	if(_bulletBroadphase != nullptr)
	{
		delete _bulletBroadphase;
		_bulletBroadphase = nullptr;
	}

	if(_bulletDispatcher != nullptr)
	{
		delete _bulletDispatcher;
		_bulletDispatcher = nullptr;
	}

	if(_bulletCollisionConfiguration != nullptr)
	{
		delete _bulletCollisionConfiguration;
		_bulletCollisionConfiguration = nullptr;
	}

}

void PhysicsEngine::addRigidBody(btRigidBody* rigidBody)
{
	_bulletWorld->addRigidBody(rigidBody);
}

void PhysicsEngine::stepSimulation(float dt)
{
	static bool firstTime = true;
	// The first dt is usually very high because of some initializations
	// TODO change vulkan dt to start with 0

	if(_bulletWorld)
	{
		//_bulletWorld->stepSimulation(dt/1000.f);
		//float dt = clock.getTimeMilliseconds()/1000.0f;
		// reset the clock to 0
		//clock.reset();

		if(!firstTime)
		{
			// Slow motion
			_bulletWorld->stepSimulation(dt/10.);
			// Normal motion
			//_bulletWorld->stepSimulation(dt);
		}
		else
			firstTime=false;
	}
}

bool PhysicsEngine::raycast(glm::vec3 startPosition, glm::vec3 direction, RayResult& output)
{
	if(!_bulletWorld) 
		return false;
	
	// Get the picking ray from where was clicked
	btVector3 rayTo = PhysicsEngine::glm2bt(direction);
	btVector3 rayFrom = PhysicsEngine::glm2bt(startPosition);

	// Create raycast callback object
	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom,rayTo);	
	
	// Perform raycast
	_bulletWorld->rayTest(rayFrom,rayTo,rayCallback);
	
	// Did we hit something?
	if(rayCallback.hasHit())
	{
		// If so, get the rigid body we hit
		btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (!pBody)
			return false;

		// Prevent us from picking objects like the ground plane
		if (pBody->isStaticObject() || pBody->isKinematicObject()) 
			return false;

		// Set the result data
		output.body = pBody;
		output.hitPoint = bt2glm(rayCallback.m_hitPointWorld);
		return true;
	}
	
	// Didn't hit anything
	return false;
}

//---------- Static functions ----------//
glm::vec3 PhysicsEngine::getMouseClickRay(int x, int y, int width, int height, glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camUp)
{
	const float nearPlane = 0.1f;
	const float farPlane = 1000.0f;

	// Calculate fielf-of-view
	float tanFov = 1.0f;//1.0f/nearPlane;
	float fov = btScalar(2.0)*btAtan(tanFov);

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

btVector3 PhysicsEngine::glm2bt(glm::vec3 vec)
{
	return { vec.x, vec.y, vec.z };
}

glm::vec3 PhysicsEngine::bt2glm(btVector3 vec)
{
	return { vec.x(), vec.y(), vec.z() };
}

btQuaternion PhysicsEngine::glm2bt(glm::quat quat)
{
	return { quat.x, quat.y, quat.z, quat.w };
}

glm::quat PhysicsEngine::bt2glm(btQuaternion quat)
{
	return { quat.w(), quat.x(), quat.y(), quat.z() };
}
