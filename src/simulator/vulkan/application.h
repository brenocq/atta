#ifndef APPLICATION_H
#define APPLICATION_H

#include "window.h"
#include "instance.h"
#include "debugMessenger.h"

class Application
{
	public:
	Application();
	~Application();

	void run();

	private:
	Window* _window;
	Instance* _instance;
	DebugMessenger* _debugMessenger;
};

#endif// APPLICATION_H
