//--------------------------------------------------
// Robot Simulator
// simulator.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "simulator.h"

#include "objects/basic/importedObject.h"
#include "objects/basic/plane.h"
#include "objects/basic/box.h"
#include "objects/basic/sphere.h"
#include "objects/basic/cylinder.h"
#include "physics/constraints/fixedConstraint.h"
#include "physics/constraints/hingeConstraint.h"

Simulator::Simulator()
{
	_scene = new Scene();
	// Load objects
	_scene->loadObject("wheel");

	// Create object instances
	Box* ground = new Box("Ground", {0,-1,0}, {0,0,0}, {200, 2, 200}, 0.0f, {0.8,0.8,0.8});
	Cylinder* test1 = new Cylinder("Cylinder", {0,1,0}, {0,0,0}, {0.5, 0.5,0.5}, 1.1f, {0.8,0.8,0.8});
	Sphere* test2 = new Sphere("Sphere", {0,0.5,0}, {0,0,0}, 0.2, 1.1f, {0.8,0.5,0.2});

	// Create demo robot (ttzinho)
	_ttzinho = new Ttzinho();

	_scene->addObject((Object*)ground);// Add a simple object
	_scene->addComplexObject(_ttzinho->getObject());// Add the object and its children
	_scene->addObject((Object*)test1);// Add a simple object
	_scene->addObject((Object*)test2);// Add a simple object
	_scene->linkObjects();

	_debugDrawer = new DebugDrawer(_scene);

	_vulkanApp = new Application(_scene);
	_vulkanApp->onDrawFrame = [this](float dt){ onDrawFrame(dt); };
	_vulkanApp->onRaycastClick = [this](glm::vec3 pos, glm::vec3 ray){ onRaycastClick(pos, ray); };
}

Simulator::~Simulator()
{
	if(_vulkanApp != nullptr)
	{
		delete _vulkanApp;
		_vulkanApp = nullptr;
	}

	if(_scene != nullptr)
	{
		delete _scene;
		_scene = nullptr;
	}

	if(_debugDrawer != nullptr)
	{
		delete _debugDrawer;
		_debugDrawer = nullptr;
	}

	if(_ttzinho != nullptr)
	{
		delete _ttzinho;
		_ttzinho = nullptr;
	}
}

void Simulator::run()
{
	_vulkanApp->run();
}

void Simulator::onDrawFrame(float dt)
{
	//btVector3 old = _scene->getObjects()[9]->getObjectPhysics()->getRigidBody()->getLinearVelocity();
	//_scene->getObjects()[9]->getObjectPhysics()->getRigidBody()->setLinearVelocity(btVector3(-1.0f, old.y(), old.z()));
	//_scene->getObjects()[9]->getObjectPhysics()->getRigidBody()->setAngularVelocity(btVector3(0.0f, 1.0f, 0.0f));
}

void Simulator::onRaycastClick(glm::vec3 pos, glm::vec3 ray)
{
	//_scene->addLine(pos, pos+ray, {rand()%255/255.f,rand()%255/255.f,rand()%255/255.f});

	//PhysicsEngine::RayResult result;
	//if(!_scene->getPhysicsEngine()->raycast(pos, ray, result))
	//	return;

	//Object* object = _scene->getObjectFromPhysicsBody(result.body);
	//
	//if(object != nullptr)
	//{
	//	printf("Hit something! %s (%f, %f, %f)\n", object->getName().c_str(), result.hitPoint.x, result.hitPoint.y, result.hitPoint.z);
	//	fflush(stdout);
	//}
}

