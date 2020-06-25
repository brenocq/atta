//--------------------------------------------------
// Robot Simulator
// simulator.cpp
// Date: 21/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "simulator.h"

Simulator::Simulator()
{
	_vulkanApp = new Application();
}

Simulator::~Simulator()
{
	delete _vulkanApp;
}

void Simulator::run()
{
	_vulkanApp->run();
}
