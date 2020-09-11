//--------------------------------------------------
// Robot Simulator
// plane.cpp
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "plane.h"

Plane::Plane(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec2 size, float mass):
	Object(name, position, rotation, {1,1,1}, mass), _size(size)
{
	_type = "Plane";
	_physics = new ObjectPhysics(_position, _rotation, mass);
}

Plane::~Plane()
{
}
