//--------------------------------------------------
// Robot Simulator
// distant.cpp
// Date: 2021-02-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "distant.h"

namespace atta
{
	DistantLight::DistantLight(CreateInfo info):
		Object({info.name, {0,0,0}, {0,0,0}, {1,1,1}, 0}),
		_radiance(info.radiance), _direction(info.direction)
	{
		Object::setType("DistantLight");
		_isLight = true;
	}

	DistantLight::~DistantLight()
	{
	}
}
