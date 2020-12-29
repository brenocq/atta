//--------------------------------------------------
// Robot Simulator
// sphere.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "sphere.h"
#include "simulator/physics/shapes/sphereShape.h"

Sphere::Sphere(std::string name, glm::vec3 position, glm::vec3 rotation, float radius, float mass, glm::vec3 color):
	Object(name, position, rotation, {radius*2, radius*2, radius*2}, mass), _color(color)
{
	_type = "Sphere";
	_model = new Model("atta::sphere");

	_orientation.fromEuler(atta::radians(atta::vec3(_rotation)));

	_bodyPhysics = new Body(&_position, &_orientation, mass);
	_bodyPhysics->addShape(new atta::phy::SphereShape(vec3(), quat(), radius+0.001));
}

Sphere::~Sphere()
{
}
