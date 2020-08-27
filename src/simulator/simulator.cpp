//--------------------------------------------------
// Robot Simulator
// simulator.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "simulator.h"

#include "objects/basic/importedObject.h"
#include "objects/basic/plane.h"

Simulator::Simulator()
{
	_scene = new Scene();
	// Load objects
	_scene->loadObject("cube_multi");

	// Create object instances
	Plane* ground = new Plane("Ground", {0,0,0}, {0,0,0}, {100, 100}, 0.0f);
	ImportedObject* cube1 = new ImportedObject("FirstCube", "cube_multi", {0.5,3,0}, {0,0,0}, {1,1,1}, 1.0f);
	ImportedObject* cube2 = new ImportedObject("SecondCube", "cube_multi", {0,0.7,0}, {0,0,0}, {1,1,1}, 0.0f);
	ImportedObject* cube3 = new ImportedObject("ThirdCube", "cube_multi", {-0.5,3,0}, {0,0,0}, {1,1,1}, 2.0f);

	_scene->addObject((Object*)ground);
	_scene->addObject((Object*)cube1);
	_scene->addObject((Object*)cube2);
	_scene->addObject((Object*)cube3);

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
	_scene->updatePhysics(dt);
}

void Simulator::onRaycastClick(glm::vec3 pos, glm::vec3 ray)
{
	_scene->addLine(pos, pos+ray, {1,1,1});
	_scene->updateLineBuffer();
	printf("Mouse click ray: (%f,%f,%f)->(%f,%f,%f)\n", pos.x, pos.y, pos.z, ray.x, ray.y, ray.z);

	PhysicsEngine::RayResult result;
	if(!_scene->getPhysicsEngine()->raycast(pos, ray, result))
		return;

	printf("Hit something!");
}
