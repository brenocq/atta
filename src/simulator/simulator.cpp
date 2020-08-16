//--------------------------------------------------
// Robot Simulator
// simulator.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "simulator.h"

Simulator::Simulator()
{
	_scene = new Scene();
	// Load objects
	_scene->loadObject("cube_multi");

	// Create object instances
	Object* cube1 = new Object("FirstCube", "cube_multi", {0.5,3,0});
	Object* cube2 = new Object("FirstCube", "cube_multi", {0,0.5,0}, {90,0,0});

	_scene->addObject(cube1);
	_scene->addObject(cube2);

	_vulkanApp = new Application(_scene);
	_vulkanApp->onDrawFrame = [this](float dt){ onDrawFrame(dt); };
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
