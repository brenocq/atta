//--------------------------------------------------
// Robot Simulator
// box.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "box.h"
#include "simulator/physics/shapes/shapes.h"

Box::Box(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 size, float mass, glm::vec3 color):
	Object(name, position, rotation, size, mass), _color(color)
{
	_type = "Box";
	_model = new Model("box");
	_bodyPhysics = new Body(_position, _rotation, mass);
	_bodyPhysics->addShape(new atta::phy::BoxShape());
}

Box::~Box()
{
}
