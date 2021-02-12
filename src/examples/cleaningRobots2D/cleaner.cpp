//--------------------------------------------------
// Atta Cleaning Robots 2D Example
// cleaner.cpp
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "cleaner.h"
#include "simulator/objects/basics/basics.h"

Cleaner::Cleaner(atta::vec3 position, atta::vec3 rotation):
	_position(position), _theta(atta::radians(rotation.y)), _vel(0.5)
{
	atta::Cylinder::CreateInfo cylinderInfo = {
		.name = "Cleaner",
		.position = position,
		.rotation = rotation,
		.height = 0.05,
		.radius = 0.1,
		.material = {
			.albedo = {.2,.2,.2}
		}
	};
	std::shared_ptr<atta::Cylinder> cylinder = std::make_shared<atta::Cylinder>(cylinderInfo);

	_rootObject = cylinder;
}

Cleaner::~Cleaner()
{

}

void Cleaner::run(float dt)
{
	_position.x -= dt*_vel*cos(_theta);
	_position.z -= dt*_vel*sin(_theta);
	_theta += dt*(2*M_PI)/5;// 1 complete rotation every 5 seconds
	if(_theta>2*M_PI)_theta-=2*M_PI;

	updateRootObject();
}

void Cleaner::updateRootObject()
{
	// Update position
	_rootObject->setPosition(_position);

	// Update rotation
	atta::quat ori = atta::eulerToQuat(atta::vec3(0, _theta, 0));
	_rootObject->setOrientation(ori);
}
