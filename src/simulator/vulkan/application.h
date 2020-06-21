#ifndef APPLICATION_H
#define APPLICATION_H

#include "window.h"
#include "instance.h"

class Application
{
	public:
	Application();
	~Application();

	void run();

	private:
	Window* _window;
	Instance* _instance;
};

#endif// APPLICATION_H
