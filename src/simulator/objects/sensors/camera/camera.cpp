//--------------------------------------------------
// Robot Simulator
// camera.h
// Date: 2020-10-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "camera.h"
Camera::Camera(std::string name, glm::vec3 position, glm::vec3 rotation, CameraInfo cameraInfo):
	Object(name, position, rotation, {1,1,1}), 
	_info(cameraInfo)
{
	_type = "Camera";
	_buffer = std::vector<uint8_t>(_info.width*_info.height*3);

	_physics = new ObjectPhysics(_position, _rotation, 1);
}

Camera::~Camera()
{

}

void Camera::createCamera(Application* application)
{
	// Create camera pipeline

}

void Camera::takePicture()
{
	// Transfer image buffer from GPU memory to buffer

}
