//--------------------------------------------------
// Robot Simulator
// box.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "box.h"
#include "simulator/physics/shapes/shapes.h"
#include "simulator/math/math.h"

Box::Box(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 size, float mass, glm::vec3 color):
	Object(name, position, rotation, size, mass), _color(color)
{
	_type = "Box";
	_model = new Model("box");

	_orientation.fromEuler(atta::radians(atta::vec3(_rotation)));

	_bodyPhysics = new Body(&_position, &_orientation, mass);
	_bodyPhysics->addShape(new atta::phy::BoxShape(vec3(), quat(), _scale));
}

Box::~Box()
{
}
