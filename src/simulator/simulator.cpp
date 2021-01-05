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
#include "simulator/helpers/evaluator.h"

Simulator::Simulator()
{
	Evaluator::reportCounter("test", 2);
	Evaluator::reportCounter("alo", 1);
	Evaluator::reportCounter("test");
	Evaluator::reportCounter("alo", 5);
	Evaluator::showReports();
	Evaluator::clear();
	Evaluator::showReports();

	_scene = new Scene();
	// Load objects
	_scene->loadObject("wheel");

	// Create object instances
	Box* ground = new Box("Ground", {0,-1,0}, {0,0,0}, {200, 2, 200}, 0.0f, {1.0,1.0,1.0});
	//Cylinder* cylinder = new Cylinder("Obstacle", {0,1,0}, {0,0,0}, {1,1,1}, 1.0f, {0.5f, 0.1f, 0.1f});
	//cylinder->setSelection(Object::ObjectSelection::SELECTED);
	Box* box = new Box("Box", {0,1,0}, {0,45,45}, {1,1,1}, 1.0f, {0.5f, 0.1f, 0.1f});
	//box->setSelection(Object::ObjectSelection::SELECTED);
	Sphere* sphere = new Sphere("Sphere", {-2,1,0}, {90,0,0}, 0.5f, 1.0f, {0.2f, 0.2f, 0.8f});
	Cylinder* cylinder = new Cylinder("Cylinder", {2,1,0}, {0,0,0}, {1,1,1}, 1.0f, {0.1f, 0.5f, 0.5f});
	Capsule* capsule = new Capsule("Capsule", {0,3,0}, {0,0,0}, {1,1,1}, 1.0f, {0.5f, 0.5f, 0.1f});
	//sphere->setSelection(Object::ObjectSelection::SELECTED);

	// Create mini cleaner robot
	//_miniCleaners.push_back(new MiniCleaner({1,0.1,0}, {0,90,0}, {1,0,0}));
	//_miniCleaners.push_back(new MiniCleaner({1,0.1,1}, {0,0,0}, {0,1,0}));
	//_miniCleaners.push_back(new MiniCleaner({0,0.1,1}, {0,45,0}, {0,0,1}));
	//_miniCleaners.push_back(new MiniCleaner({0,0.1,0}, {0,-45,0}, {0,1,1}));

	// Add objects to the scene
	_scene->addObject(ground);
	//for(auto& miniCleaner : _miniCleaners)
	//	_scene->addObject((Object*)miniCleaner);
	_scene->addObject(box);
	_scene->addObject(sphere);
	_scene->addObject(cylinder);
	_scene->addObject(capsule);
	// Link objects to each other (physics)
	_scene->linkObjects();

	_drawer = new Drawer(_scene);

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

	if(_drawer != nullptr)
	{
		delete _drawer;
		_drawer = nullptr;
	}

	//for(auto& miniCleaner : _miniCleaners)
	//{
	//	if(miniCleaner != nullptr)
	//	{
	//		delete miniCleaner;
	//		miniCleaner = nullptr;
	//	}
	//}
}

void Simulator::run()
{
	_vulkanApp->run();
}

void Simulator::onDrawFrame(float dt)
{
	for(auto& miniCleaner : _miniCleaners)
		miniCleaner->run(dt);

	_drawer->drawPhysicsShapes();
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

