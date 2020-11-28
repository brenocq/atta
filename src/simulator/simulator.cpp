//--------------------------------------------------
// Robot Simulator
// simulator.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "simulator.h"

#include "simulator/objects/basics/basics.h"
#include "simulator/physics/constraints/fixedConstraint.h"
#include "simulator/physics/constraints/hingeConstraint.h"

Simulator::Simulator()
{
	_scene = new Scene();
	// Load objects
	_scene->loadObject("wheel");

	// Create object instances
	Box* ground = new Box("Ground", {0,-1,0}, {0,0,0}, {200, 2, 200}, 0.0f, {0.8,0.8,0.8});
	Cylinder* cylinder = new Cylinder("Obstacle", {7,1,0}, {0,0,0}, {1,1,1}, 0.0f, {0.5f, 0.1f, 0.1f});
	cylinder->setSelection(Object::ObjectSelection::SELECTED);

	// Create mini cleaner robot
	_miniCleaners.push_back(new MiniCleaner({1,0.1,0}, {0,90,0}, {1,0,0}));
	_miniCleaners.push_back(new MiniCleaner({1,0.1,1}, {0,0,0}, {0,1,0}));
	_miniCleaners.push_back(new MiniCleaner({0,0.1,1}, {0,45,0}, {0,0,1}));
	_miniCleaners.push_back(new MiniCleaner({0,0.1,0}, {0,-45,0}, {0,1,1}));

	// Add objects to the scene
	_scene->addObject((Object*)ground);
	for(auto& miniCleaner : _miniCleaners)
		_scene->addObject((Object*)miniCleaner);
	_scene->addObject((Object*)cylinder);
	// Link objects to each other (physics)
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

	for(auto& miniCleaner : _miniCleaners)
	{

		if(miniCleaner != nullptr)
		{
			delete miniCleaner;
			miniCleaner = nullptr;
		}
	}
}

void Simulator::run()
{
	_vulkanApp->run();
}

void Simulator::onDrawFrame(float dt)
{
	for(auto& miniCleaner : _miniCleaners)
		miniCleaner->run(dt);
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

