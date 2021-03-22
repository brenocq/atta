//--------------------------------------------------
// Robot Simulator
// point.cpp
// Date: 2021-02-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/lights/point.h>

namespace atta
{
	PointLight::PointLight(CreateInfo info):
		Object({info.name, info.position, {0,0,0}, {1,1,1}, 0}),
		_intensity(info.intensity)
	{
		Object::setType("PointLight");
		_isLight = true;
	}

	PointLight::~PointLight()
	{
	}
}
