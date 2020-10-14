//--------------------------------------------------
// Robot Simulator
// displayTFT144.cpp
// Date: 2020-10-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "displayTFT144.h"
#include "simulator/objects/basic/plane.h"
#include "simulator/physics/constraints/hingeConstraint.h"

DisplayTFT144::DisplayTFT144(std::string name, glm::vec3 position, glm::vec3 rotation):
	Object(name, position, rotation, {0.1,0.1,0.1})
{
	_type = "Display";
	_model = new Model("box");
	_physics = new ObjectPhysics(_position, _rotation, 0.2);

	Plane* plane = new Plane("Screen", {0,0.251,0}, {0,0,0}, {0.1, 0.1}, 0.01f, {0,0,0.8});

	addChild(plane, new HingeConstraint({0.2,0,0}, {0,0,90}));
}

DisplayTFT144::~DisplayTFT144()
{

}
