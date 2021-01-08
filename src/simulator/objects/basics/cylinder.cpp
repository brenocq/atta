//--------------------------------------------------
// Robot Simulator
// cylinder.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "cylinder.h"

namespace atta
{
	Cylinder::Cylinder(CreateInfo info):
		Object({info.name, info.position, info.rotation, {1,1,1}, info.mass}), _color(info.color)
	{
		Object::setType("Cylinder");
	}

	Cylinder::~Cylinder()
	{
	}
}
