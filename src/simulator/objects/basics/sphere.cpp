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
		Object({info.name, info.position, info.rotation, {info.radius*2, info.radius*2, info.radius*2}, info.mass}), _color(info.color)
	{
		Object::setType("Sphere");
	}

	Sphere::~Sphere()
	{
	}
}
