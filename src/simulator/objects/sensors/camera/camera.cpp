//--------------------------------------------------
// Robot Simulator
// camera.h
// Date: 2020-10-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "camera.h"

namespace atta
{
	Camera::Camera(CreateInfo info):
		Object({info.name, info.position, info.rotation, info.scale}), 
		_renderingType(info.renderingType), 
		_width(info.width), _height(info.height),
		_fov(info.fov)
	{
		Object::setType("Camera");
		_buffer = std::vector<uint8_t>(_width*_height*3);
		//_bodyPhysics = new Body(&_position, &_orientation, 1.0f);

		// When the vulkan application is created it creates a raytracing/rasterization pipeline to each camera
	}

	Camera::~Camera()
	{

	}

	void Camera::takePicture()
	{
		// TODO Transfer image buffer from GPU memory to buffer

	}
}
