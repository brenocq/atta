//--------------------------------------------------
// Robot Simulator
// object.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "object.h"

Object::Object(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float mass):
	_name(name), _position(position), _rotation(rotation), _scale(scale), _mass(mass), _physics(nullptr), _type("Object")
{
}

Object::~Object()
{
	if(_physics != nullptr)
	{
		delete _physics;
		_physics = nullptr;
	}
}

glm::mat4 Object::getModelMat()
{
	if(_physics != nullptr)
	{
		_position = _physics->getPosition();
		_rotation = _physics->getRotation();
	}

	glm::mat4 mat = glm::mat4(1);
	
	mat = glm::scale(mat, glm::vec3(1.0));
	mat = glm::translate(mat, _position);
	mat = glm::rotate(mat, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
	
	return mat;
}
