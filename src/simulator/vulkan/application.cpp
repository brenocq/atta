#include "application.h"

Application::Application()
{
	_window = new Window();
	_instance = new Instance();
	_debugMessenger = new DebugMessenger(_instance);
}

Application::~Application()
{
	delete _debugMessenger;
	delete _instance;
	delete _window;
}

void Application::run()
{
	_window->loop();
}
