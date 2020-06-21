#include "application.h"

Application::Application()
{
	_window = new Window();
	_instance = new Instance();
}

Application::~Application()
{
	delete _instance;

	delete _window;
}

void Application::run()
{
	_window->loop();
}
