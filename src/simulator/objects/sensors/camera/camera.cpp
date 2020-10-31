//--------------------------------------------------
// Robot Simulator
// camera.h
// Date: 2020-10-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "camera.h"
Camera::Camera(std::string name, glm::vec3 position, glm::vec3 rotation):
	Object(name, position, rotation, {1,1,1}), _width(200), _height(200)
{

}

Camera::~Camera()
{

}

void Camera::createCamera(Application* application)
{

}
