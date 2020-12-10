//--------------------------------------------------
// Robot Simulator
// sphere.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "sphere.h"

Sphere::Sphere(std::string name, glm::vec3 position, glm::vec3 rotation, float radius, float mass, glm::vec3 color):
	Object(name, position, rotation, {radius*2, radius*2, radius*2}, mass), _color(color)
{
	_type = "Sphere";
	_model = new Model("sphere");
	_bodyPhysics = new Body(_position, _rotation, mass);
}

Sphere::~Sphere()
{
}
