//--------------------------------------------------
// Robot Simulator
// object.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "object.h"
#include <iostream>


int Object::_qtyIds = 0;
Object::Object(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float mass):
	_name(name), _position(position), _rotation(rotation), _scale(scale), _mass(mass), _physics(nullptr), _type("Object")
{
	_id = _qtyIds++;
	_static = _mass > 0;
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
	
	mat = glm::translate(mat, _position);
	mat = glm::rotate(mat, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
	mat = glm::scale(mat, _scale);
	
	return mat;
}

void Object::setPosition(glm::vec3 position)
{
	_position = position;
	if(_physics!=nullptr)
		_physics->setPosition(position);
}

void Object::setRotation(glm::vec3 rotation)
{
	_rotation = rotation;
	if(_physics!=nullptr)
		_physics->setRotation(rotation);
}

void Object::setStatic(bool stat)
{
	// TODO
	//if(stat)
	//{
	//	std::cout << "STATIC\n";
	//	_static = true;
	//	_physics->setMass(0);
	//}
	//else
	//{
	//	std::cout << "dynamic\n";
	//	_static = false;
	//	
	//	if(_mass==0)
	//		_mass = 1.0f;

	//	_physics->setMass(_mass);
	//}
}
