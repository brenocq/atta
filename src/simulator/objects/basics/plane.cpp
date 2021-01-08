//--------------------------------------------------
// Robot Simulator
// plane.cpp
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "plane.h"

namespace atta
{
	Plane::Plane(CreateInfo info):
		Object({info.name, info.position, info.rotation, {info.size.x,1,info.size.y}, info.mass}), _color(info.color)
	{
		Object::setType("Plane");
	}

	Plane::~Plane()
	{
	}
}
