//--------------------------------------------------
// Robot Simulator
// sphere.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "sphere.h"

namespace atta
{
	Sphere::Sphere(CreateInfo info):
		Object({info.name, info.position, vec3(0,0,0), {info.radius*2, info.radius*2, info.radius*2}, info.mass})
	{
		Object::setType("Sphere");

		//----- Create model -----//
		Model::CreateInfo modelInfo = {
			.meshName = "atta::sphere",
			.material = info.material
		};

		_model = std::make_shared<Model>(modelInfo);
		_bodyPhysics->addShape(std::make_shared<phy::SphereShape>(vec3(), quat(), 1.0));
	}

	Sphere::~Sphere()
	{
	}
}
