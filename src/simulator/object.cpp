//--------------------------------------------------
// Robot Simulator
// object.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "object.h"

Object::Object(std::string name, std::string fileName, glm::vec3 position, glm::vec3 rotation):
	_name(name), _position(position), _rotation(rotation)
{
	_model = new Model(fileName);
	_physics = new ObjectPhysics(new btBoxShape(btVector3(1,1,1)));
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
	glm::mat4 mat = glm::mat4(1);
	
	mat = glm::scale(mat, glm::vec3(1.0));
	mat = glm::translate(mat, _position);
	mat = glm::rotate(mat, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
	
	return mat;
}
