//--------------------------------------------------
// Robot Simulator
// simulator.h
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <vector>
#include "scene.h"
#include "vulkan/application.h"

class Simulator
{
	public:
		Simulator();
		~Simulator();

		void run();

	private:
		void onDrawFrame(float dt);

		Scene* _scene;
		Application* _vulkanApp;
};

#endif// SIMULATOR_H
