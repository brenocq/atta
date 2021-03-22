//--------------------------------------------------
// Robot Simulator
// spot.cpp
// Date: 2021-02-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/lights/spot.h>

namespace atta
{
	SpotLight::SpotLight(CreateInfo info):
		Object({info.name, info.position, {0,0,0}, {1,1,1}, 0}),
		_intensity(info.intensity), _direction(info.direction),
		_falloffStart(info.falloffStartAngle), _totalWidth(info.maxAngle)
	{
		Object::setType("SpotLight");
		_isLight = true;
	}

	SpotLight::~SpotLight()
	{
	}
}
