//--------------------------------------------------
// Robot Simulator
// cylinder.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "cylinder.h"

Cylinder::Cylinder(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float mass, glm::vec3 color):
	Object(name, position, rotation, scale, mass), _color(color)
{
	_type = "Cylinder";
	_model = new Model("atta::cylinder");
	_bodyPhysics = new Body(&_position, &_orientation, mass);
	_bodyPhysics->addShape(new atta::phy::CylinderShape(atta::vec3(), atta::quat(), atta::vec3(1,1,1)));
}

Cylinder::~Cylinder()
{
}
