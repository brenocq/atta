//--------------------------------------------------
// Robot Simulator
// capsule.cpp
// Date: 2020-12-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "capsule.h"

Capsule::Capsule(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float mass, glm::vec3 color):
	Object(name, position, rotation, scale, mass), _color(color)
{
	_type = "Capsule";
	_model = new Model("atta::capsule");
	_bodyPhysics = new Body(&_position, &_orientation, mass);
	_bodyPhysics->addShape(new atta::phy::CapsuleShape(atta::vec3(), atta::quat(), atta::vec3(1,1,1)));
}

Capsule::~Capsule()
{
}
