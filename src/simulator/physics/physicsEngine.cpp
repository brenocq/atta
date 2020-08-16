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
			//printf("step: %f\n", dt);
			//_bulletWorld->stepSimulation(dt);
		}
		else
			firstTime=false;
	}
}

btVector3 PhysicsEngine::glm2bt(glm::vec3 vec)
{
	return { vec.x, vec.y, vec.z };
}

glm::vec3 PhysicsEngine::glm2bt(btVector3 vec)
{
	return { vec.x(), vec.y(), vec.z() };
}

btQuaternion PhysicsEngine::glm2bt(glm::quat quat)
{
	return { quat.x, quat.y, quat.z, quat.w };
}

glm::quat PhysicsEngine::glm2bt(btQuaternion quat)
{
	return { quat.w(), quat.x(), quat.y(), quat.z() };
}
