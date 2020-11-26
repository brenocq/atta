//--------------------------------------------------
// Robot Simulator
// miniCleaner.h
// Date: 2020-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MINI_CLEANER_H
#define MINI_CLEANER_H

#include <string>
#include <vector>
#include "simulator/object.h"
#include "simulator/objects/others/display/display.h"
#include "simulator/objects/sensors/camera/camera.h"

class MiniCleaner : public Object
{
	public:
		MiniCleaner(glm::vec3 position={0,0,0}, glm::vec3 rotation={0,0,0}, glm::vec3 color = {0, 1, 1});
		~MiniCleaner();

		void run(float dt);

	private:
		Display* _display;
		Camera* _camera;
};

#endif// MINI_CLEANER_H

