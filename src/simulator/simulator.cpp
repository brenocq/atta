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
	Box* ground = new Box("Ground", {0,-1,0}, {0,0,0}, {200, 2, 200}, 0.0f, {0,0,0});
	ImportedObject* wheel = new ImportedObject("Wheel test", "wheel", {-0.4,0.06,0}, {0,0,90}, {1,1,1}, 0.1f, new btCylinderShape(btVector3(0.06, 0.01, 0.06)));
	//ImportedObject* cube = new ImportedObject("FirstCube", "cube_multi", {0.5,3,0}, {0,0,0}, {1,1,1}, 1.0f);
	/*
	Sphere* body = new Sphere("Body", {0,5,0}, {0,0,0}, 2.0f, 2.0f, {0,0.8,0});
	Sphere* head = new Sphere("Head", {0,7.5,0}, {0,0,0}, 0.5f, 2.0f, {0.8,0,0.8});
	Cylinder* leg1 = new Cylinder("Leg1", {0.8,2,0}, {0,0,0}, {1,2,1}, 2.0f, {0,0,0.8});
	Cylinder* leg2 = new Cylinder("Leg2", {-0.8,2,0}, {0,0,0}, {1,2,1}, 2.0f, {0,0,0.8});
	Box* foot1 = new Box("Foot1", {0.8,0.75,0.5}, {0,0,0}, {1,0.5,2}, 2.0f, {0.8,0,0});
	Box* foot2 = new Box("Foot2", {-0.8,0.75,0.5}, {0,0,0}, {1,0.5,2}, 2.0f, {0.8,0,0});
	Cylinder* arm1 = new Cylinder("Arm1", {2.25,4,0}, {0,0,0}, {0.5,2,0.5}, 2.0f, {0,0,0.8});
	Cylinder* arm2 = new Cylinder("Arm2", {-2.25,4,0}, {0,0,0}, {0.5,2,0.5}, 2.0f, {0,0,0.8});
	Cylinder* wheel = new Cylinder("Wheel", {-5,2,0}, {90,0,0}, {1.0,0.5,1.0}, 0.1f, {0,0,0.8});

	// Set parents
	body->addChild(head, new FixedConstraint({0,2.5,0}, {0,0,0}));
	body->addChild(leg1, new FixedConstraint({0.8,-3,0}, {0,0,0}));
	body->addChild(leg2, new FixedConstraint({-0.8,-3,0}, {0,0,0}));
	leg1->addChild(foot1, new FixedConstraint({0,-1.25,0.5}, {0,0,0}));
	leg2->addChild(foot2, new FixedConstraint({0,-1.25,0.5}, {0,0,0}));
	body->addChild(arm1, new FixedConstraint({2.25,-1,0}, {0,0,0}));
	body->addChild(arm2, new FixedConstraint({-2.25,-1,0}, {0,0,0}));
	_scene->addComplexObject((Object*)body);
	*/

	// Create demo robot (ttzinho)
	_ttzinho = new Ttzinho();


	_scene->addObject((Object*)ground);// Add a simple object
	_scene->addComplexObject(_ttzinho->getObject());// Add the object and its children
	_scene->addObject((Object*)wheel);// Add a simple object
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

	HingeConstraint* h = (HingeConstraint*)_ttzinho->getObject()->getChildren()[0]->getParentConstraint();
	h->getConstraint()->setMotorTarget(10, dt);
}

void Simulator::onRaycastClick(glm::vec3 pos, glm::vec3 ray)
{
	//_scene->addLine(pos, pos+ray, {rand()%255/255.f,rand()%255/255.f,rand()%255/255.f});

	PhysicsEngine::RayResult result;
	if(!_scene->getPhysicsEngine()->raycast(pos, ray, result))
		return;

	Object* object = _scene->getObjectFromPhysicsBody(result.body);
	
	if(object != nullptr)
	{
		printf("Hit something! %s (%f, %f, %f)\n", object->getName().c_str(), result.hitPoint.x, result.hitPoint.y, result.hitPoint.z);
		fflush(stdout);
	}
}

