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

Simulator::Simulator()
{
	_scene = new Scene();
	// Load objects
	_scene->loadObject("cube_multi");

	// Create object instances
	Box* ground = new Box("Ground", {0,-1,0}, {0,0,0}, {200, 2, 200}, 0.0f, {0,0,0});
	ImportedObject* cube = new ImportedObject("FirstCube", "cube_multi", {0.5,3,0}, {0,0,0}, {1,1,1}, 1.0f);
	Box* box = new Box("BoxTest", {-0.5,5,0}, {0,0,0}, {2,1,2}, 2.0f, {0.8,0,0});
	Sphere* sphere = new Sphere("SphereTest", {-0.5,5,0}, {0,0,0}, 2.0f, 2.0f, {0.8,0,0});
	Cylinder* cylinder = new Cylinder("CylinderTest", {1.0,5,0}, {0,0,0}, {1,2,1}, 2.0f, {0.8,0,0});

	//btTransform frameInA;
	//frameInA = btTransform::getIdentity();
	//btTransform frameInB;
	//frameInB = btTransform::getIdentity();
	//frameInB.setOrigin(btVector3(btScalar(1.), btScalar(0.), btScalar(0.)));

	//btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(
	//		*cube1->getObjectPhysics()->getRigidBody(), 
	//		*cube3->getObjectPhysics()->getRigidBody(), 
	//		frameInA,
	//		frameInB,
	//		true);
	//// lock all rorations
	//dof6->setAngularLowerLimit(btVector3(0.f, 0.f, 0.f));
	//dof6->setAngularUpperLimit(btVector3(0.f, 0.f, 0.f));
	//dof6->setLinearLowerLimit(btVector3(0.f, 0.f, 0.f));
	//dof6->setLinearUpperLimit(btVector3(0.f, 0.f, 0.f));
	//	
	//_scene->getPhysicsEngine()->getWorld()->addConstraint(dof6, true);

	_scene->addObject((Object*)ground);
	_scene->addObject((Object*)cube);
	_scene->addObject((Object*)box);
	_scene->addObject((Object*)sphere);
	_scene->addObject((Object*)cylinder);

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

