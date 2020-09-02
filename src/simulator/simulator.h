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
#include "demo/ttzinho/ttzinho.h"
#include "helpers/debugDrawer.h"

class Simulator
{
	public:
		Simulator();
		~Simulator();

		void run();

	private:
		void onDrawFrame(float dt);
		void onRaycastClick(glm::vec3 pos, glm::vec3 ray);

		Scene* _scene;
		DebugDrawer* _debugDrawer;
		Application* _vulkanApp;

		// Example specific
		Ttzinho* _ttzinho;
};

#endif// SIMULATOR_H
