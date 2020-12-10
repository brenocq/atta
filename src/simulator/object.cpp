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
	_name(name), _type("Object"), _position(position), _rotation(rotation), _scale(scale), _mass(mass), _bodyPhysics(nullptr),
	_selection(ObjectSelection::UNSELECTED)
{
	_id = _qtyIds++;

	_bodyPhysics = nullptr;
	_model = nullptr;

	_static = _mass > 0;
	_parent = nullptr;
	_parentConstraint = nullptr;
}

Object::~Object()
{
	if(_bodyPhysics != nullptr)
	{
		delete _bodyPhysics;
		_bodyPhysics = nullptr;
	}

	if(_model != nullptr)
	{
		delete _model;
		_model = nullptr;
	}

	if(_parentConstraint != nullptr)
	{
		delete _parentConstraint;
		_parentConstraint = nullptr;
	}
}

glm::vec3 Object::getRotation()
{
	if(_bodyPhysics != nullptr)
	{
		//_rotation = glm::vec3(0,0,0);//_bodyPhysics->getRotation();
	}
	return _rotation;
}

glm::vec3 Object::getWorldPosition()
{
	glm::mat4 transformation = getModelMat();
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transformation, scale, rotation, translation, skew, perspective);

	return translation;
}

glm::vec3 Object::getWorldRotation()
{
	glm::mat4 transformation = getModelMat();
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transformation, scale, rotation, translation, skew, perspective);

	return glm::degrees(glm::eulerAngles(rotation));
}

glm::mat4 Object::getModelMat()
{
	// TODO stores current modelMat and calculates if the object has moved
	// (Go up until find updated (world) modelMat)
	if(_bodyPhysics != nullptr)
	{
		_position = _bodyPhysics->getPosition();
		//_rotation = glm::vec3(0,0,0);//_bodyPhysics->getRotation();
	}

	glm::mat4 mat = glm::mat4(1);

	if(_parent != nullptr)
	{
		glm::vec3 parentPos = _parent->getPosition();
		glm::vec3 parentRot = _parent->getRotation();
		mat = glm::translate(mat, parentPos);
		mat = glm::rotate(mat, glm::radians(parentRot.z), glm::vec3(0, 0, 1));
		mat = glm::rotate(mat, glm::radians(parentRot.y), glm::vec3(0, 1, 0));
		mat = glm::rotate(mat, glm::radians(parentRot.x), glm::vec3(1, 0, 0));
	
		std::string constraintType = _parentConstraint->getType();
		if(constraintType == "FixedConstraint")
		{
			FixedConstraint* constraint = (FixedConstraint*)_parentConstraint;
			glm::vec3 pos = constraint->getPosition();
			glm::vec3 rot = constraint->getRotation();

			mat = glm::translate(mat, pos);
			mat = glm::rotate(mat, glm::radians(rot.z), glm::vec3(0, 0, 1));
			mat = glm::rotate(mat, glm::radians(rot.y), glm::vec3(0, 1, 0));
			mat = glm::rotate(mat, glm::radians(rot.x), glm::vec3(1, 0, 0));
		}
		else if(constraintType == "HingeConstraint")
		{
			HingeConstraint* constraint = (HingeConstraint*)_parentConstraint;
			glm::vec3 pos = constraint->getPosition();
			glm::vec3 rot = constraint->getRotation();

			mat = glm::translate(mat, pos);
			mat = glm::rotate(mat, glm::radians(rot.z), glm::vec3(0, 0, 1));
			mat = glm::rotate(mat, glm::radians(rot.y), glm::vec3(0, 1, 0));
			mat = glm::rotate(mat, glm::radians(rot.x), glm::vec3(1, 0, 0));
		}
	}

	mat = glm::translate(mat, _position);
	mat = glm::rotate(mat, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
	mat = glm::rotate(mat, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
	mat = glm::scale(mat, _scale);
	
	return mat;
}

void Object::setPosition(glm::vec3 position)
{
	_position = position;
	if(_bodyPhysics!=nullptr)
		_bodyPhysics->setPosition(atta::vec3(position));
}

void Object::setRotation(glm::vec3 rotation)
{
	for(auto child : _children)
	{
		glm::vec3 rot = rotation - _rotation;
		glm::vec3 childRot = child->getRotation();
		childRot += rot;
		child->setRotation(childRot);
	}

	_rotation = rotation;
	//if(_bodyPhysics!=nullptr)
	//	_bodyPhysics->setRotation(rotation);
}

void Object::setStatic(bool stat)
{
	// TODO
	//if(stat)
	//{
	//	std::cout << "STATIC\n";
	//	_static = true;
	//	_bodyPhysics->setMass(0);
	//}
	//else
	//{
	//	std::cout << "dynamic\n";
	//	_static = false;
	//	
	//	if(_mass==0)
	//		_mass = 1.0f;

	//	_bodyPhysics->setMass(_mass);
	//}
}

void Object::setSelection(ObjectSelection sel)
{
	_selection = sel;
	for(auto child : _children)
		child->setSelection(sel);
}

void Object::addChild(Object* child, Constraint* constraint)
{
	// Can't add same child twice
	for(auto currChild : _children)
		if(currChild == child)
			return;

	// Change child parent
	Object* oldChildParent = child->getParent();
	if(oldChildParent != nullptr)
	{
		oldChildParent->removeChild(child);
	}

	child->setParent(this);
	child->setParentConstraint(constraint);
	_children.push_back(child);
}

void Object::removeChild(Object* child)
{
	for(int i=0; i<(int)_children.size(); i++)
	{
		if(_children[i] == child)
		{
			child->setParent(nullptr);
			child->setParentConstraint(nullptr);
			_children.erase(_children.begin() + i);
			return;
		}
	}
}

void Object::setParentConstraint(atta::phy::Constraint* constraint)
{
	if(_parentConstraint != nullptr)
	{
		delete _parentConstraint;
		_parentConstraint = nullptr;
	}

	_parentConstraint = constraint;
}
