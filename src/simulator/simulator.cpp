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
