//--------------------------------------------------
// Robot Simulator
// camera.h
// Date: 2020-10-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/sensors/camera/camera.h>

namespace atta
{
	Camera::Camera(CreateInfo info):
		Object({info.name, info.position, info.rotation, info.scale, info.mass, std::move(info.children)}), 
		_renderingType(info.renderingType), 
		_width(info.width), _height(info.height),
		_fov(info.fov)
	{
		Object::setType("Camera");
		_buffer = std::vector<uint8_t>(_width*_height*3);

		//----- Model -----//
		//_model = nullptr;
		Model::CreateInfo modelInfo = {
			.meshName = "atta::box",
			.material = Material::diffuse({
						.kd = {1,0,1},
						.sigma = 20,
					}),
		};
		if(info.createModel)
			_model = std::make_shared<Model>(modelInfo);

		//----- Physics -----//
		_bodyPhysics = nullptr;

		// Obs: When the scene is created, it creates a raytracing/rasterization renderer to the camera
	}

	Camera::~Camera()
	{

	}
}
