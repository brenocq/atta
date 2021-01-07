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
#include "examples/examples.h"
#include "helpers/drawer.h"

using namespace atta;

class Simulator
{
	public:
		Simulator();
		~Simulator();

		void run();


	private:
		void welcome();
		void onDrawFrame(float dt);
		void onRaycastClick(glm::vec3 pos, glm::vec3 ray);

		Scene* _scene;
		Drawer* _drawer;
		Application* _vulkanApp;

		//---------- Examples ----------//
		std::vector<MiniCleaner*> _miniCleaners;
};

#endif// SIMULATOR_H
