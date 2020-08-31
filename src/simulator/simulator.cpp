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

Simulator::Simulator()
{
	_scene = new Scene();
	// Load objects
	_scene->loadObject("cube_multi");

	// Create object instances
	Box* ground = new Box("Ground", {0,-1,0}, {0,0,0}, {200, 2, 200}, 0.0f, {0,0,0});
	//ImportedObject* cube = new ImportedObject("FirstCube", "cube_multi", {0.5,3,0}, {0,0,0}, {1,1,1}, 1.0f);
	Sphere* body = new Sphere("Body", {0,5,0}, {0,0,0}, 2.0f, 2.0f, {0,0.8,0});
	Sphere* head = new Sphere("Head", {0,7.5,0}, {0,0,0}, 0.5f, 2.0f, {0.8,0,0.8});
	Cylinder* leg1 = new Cylinder("Leg1", {0.8,2,0}, {0,0,0}, {1,2,1}, 2.0f, {0,0,0.8});
	Cylinder* leg2 = new Cylinder("Leg2", {-0.8,2,0}, {0,0,0}, {1,2,1}, 2.0f, {0,0,0.8});
	Box* foot1 = new Box("Foot1", {0.8,0.75,0.5}, {0,0,0}, {1,0.5,2}, 2.0f, {0.8,0,0});
	Box* foot2 = new Box("Foot2", {-0.8,0.75,0.5}, {0,0,0}, {1,0.5,2}, 2.0f, {0.8,0,0});
	Cylinder* arm1 = new Cylinder("Arm1", {2.25,4,0}, {0,0,0}, {0.5,2,0.5}, 2.0f, {0,0,0.8});
	Cylinder* arm2 = new Cylinder("Arm2", {-2.25,4,0}, {0,0,0}, {0.5,2,0.5}, 2.0f, {0,0,0.8});

	// Set parents
	body->addChild(head, new FixedConstraint({0,2.5,0}, {0,0,0}));
	body->addChild(leg1, new FixedConstraint({0.8,-3,0}, {0,0,0}));
	body->addChild(leg2, new FixedConstraint({-0.8,-3,0}, {0,0,0}));
	leg1->addChild(foot1, new FixedConstraint({0,-1.25,0.5}, {0,0,0}));
	leg2->addChild(foot2, new FixedConstraint({0,-1.25,0.5}, {0,0,0}));
	body->addChild(arm1, new FixedConstraint({2.25,-1,0}, {0,0,0}));
	body->addChild(arm2, new FixedConstraint({-2.25,-1,0}, {0,0,0}));

	_scene->addObject((Object*)ground);
	_scene->addObject((Object*)body);
	_scene->addObject((Object*)head);
	_scene->addObject((Object*)leg1);
	_scene->addObject((Object*)leg2);
	_scene->addObject((Object*)foot1);
	_scene->addObject((Object*)foot2);
	_scene->addObject((Object*)arm1);
	_scene->addObject((Object*)arm2);
	_scene->linkObjects();

	_vulkanApp = new Application(_scene);
	_vulkanApp->onDrawFrame = [this](float dt){ onDrawFrame(dt); };
	_vulkanApp->onRaycastClick = [this](glm::vec3 pos, glm::vec3 ray){ onRaycastClick(pos, ray); };
}

Simulator::~Simulator()
{
	delete _vulkanApp;
	delete _scene;
}

void Simulator::run()
{
	_vulkanApp->run();
}

void Simulator::onDrawFrame(float dt)
{
	//btVector3 old = _scene->getObjects()[4]->getObjectPhysics()->getRigidBody()->getLinearVelocity();
	//_scene->getObjects()[4]->getObjectPhysics()->getRigidBody()->setLinearVelocity(btVector3(1.0f, old.y(), old.z()));
	//_scene->getObjects()[3]->getObjectPhysics()->getRigidBody()->setAngularVelocity(btVector3(1.0f, 0.0f, 0.0f));

	_scene->updatePhysics(dt);
}

void Simulator::onRaycastClick(glm::vec3 pos, glm::vec3 ray)
{
	//_scene->addLine(pos, pos+ray, {rand()%255/255.f,rand()%255/255.f,rand()%255/255.f});
	//_scene->updateLineBuffer();

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

