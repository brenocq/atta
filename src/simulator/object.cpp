//--------------------------------------------------
// Robot Simulator
// object.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "object.h"

Object::Object(std::string name, std::string fileName, glm::vec3 position, glm::vec3 rotation, float mass):
	_name(name), _position(position), _rotation(rotation)
{
	_model = new Model(fileName);
	_physics = new ObjectPhysics(new btBoxShape(btVector3(0.5,0.5,0.5)), position, rotation, mass);
}

Object::~Object()
{
	if(_physics != nullptr)
	{
		delete _physics;
		_physics = nullptr;
	}

	if(_model != nullptr)
	{
		delete _model;
		_model = nullptr;
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
