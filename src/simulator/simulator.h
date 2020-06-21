#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <vector>
#include "vulkan/application.h"

class Simulator
{
	public:
	Simulator();
	~Simulator();

	void run();

	private:
	Application* _vulkanApp;
};

#endif// SIMULATOR_H
