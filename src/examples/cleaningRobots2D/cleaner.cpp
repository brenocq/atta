//--------------------------------------------------
// Atta Cleaning Robots 2D Example
// cleaner.cpp
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "cleaner.h"
#include "simulator/objects/basics/basics.h"

Cleaner::Cleaner(atta::vec3 position, atta::vec3 rotation):
	_radius(0.1), _position(position), _theta(atta::radians(rotation.y)), _vel(2)
{
	atta::Cylinder::CreateInfo cylinderInfo = {
		.name = "Cleaner",
		.position = position,
		.rotation = rotation,
		.height = 0.05,
		.radius = _radius,
		.material = atta::Material::diffuse({.kd={.2,.2,.2}, .sigma=20})
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
	if(rand()%100>50)
		_theta += dt*(2*M_PI)*_vel;
	else
		_theta -= dt*(2*M_PI)*_vel;

	if(_theta>2*M_PI)_theta-=2*M_PI;
	if(_theta<=0)_theta+=2*M_PI;

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
