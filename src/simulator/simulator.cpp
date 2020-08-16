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
	Object* cube1 = new Object("FirstCube", "cube_multi", {0.5,3,0}, {0,0,0}, 1.0f);
	Object* cube2 = new Object("SecondCube", "cube_multi", {0,0.5,0}, {0,0,0}, 0.0f);
	Object* cube3 = new Object("ThirdCube", "cube_multi", {-0.5,3,0}, {0,0,0}, 2.0f);

	_scene->addObject(cube1);
	_scene->addObject(cube2);
	_scene->addObject(cube3);

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
