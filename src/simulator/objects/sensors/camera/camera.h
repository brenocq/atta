//--------------------------------------------------
// Robot Simulator
// camera.h
// Date: 2020-10-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <vector>
#include "simulator/object.h"
#include "simulator/vulkan/application.h"

class Camera : public Object
{
	public:
		Camera(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0});
		~Camera();

		void createCamera(Application* application);

	private:
		int _width;
		int _height;
};

#endif// CAMERA_H
