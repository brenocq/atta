//--------------------------------------------------
// Robot Simulator
// plane.cpp
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "plane.h"

Plane::Plane(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec2 size, float mass, glm::vec3 color):
	Object(name, position, rotation, {size.x,1,size.y}, mass), _size(size), _color(color)
{
	_type = "Plane";
	_model = new Model("plane");
	_physics = new ObjectPhysics(_position, _rotation, mass);
}

Plane::~Plane()
{
}
