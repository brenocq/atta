//--------------------------------------------------
// Robot Simulator
// object.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "object.h"
#include <iostream>

int Object::_qtyIds = 0;
Object::Object(std::string name, atta::vec3 position, atta::vec3 rotation, atta::vec3 scale, float mass):
	_name(name), _type("Object"), _position(position), _rotation(rotation), _scale(scale), _mass(mass), _bodyPhysics(nullptr),
	_selection(ObjectSelection::UNSELECTED)
{
	_orientation.fromEuler(atta::radians(rotation));

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

atta::vec3 Object::getRotation()
{
	if(_bodyPhysics != nullptr)
	{
		//_rotation = atta::vec3(0,0,0);//_bodyPhysics->getRotation();
	}
	return _rotation;
}

atta::vec3 Object::getWorldPosition()
{
	//atta::mat4 transformation = getModelMat();
	//atta::vec3 scale;
	//atta::quat rotation;
	//atta::vec3 translation;
	//atta::vec3 skew;
	//atta::vec4 perspective;
	//atta::decompose(transformation, scale, rotation, translation, skew, perspective);

	return atta::vec3();
}

atta::vec3 Object::getWorldRotation()
{
	//atta::mat4 transformation = getModelMat();
	//atta::vec3 scale;
	//atta::quat rotation;
	//atta::vec3 translation;
	//atta::vec3 skew;
	//atta::vec4 perspective;
	//atta::decompose(transformation, scale, rotation, translation, skew, perspective);

	//return atta::degrees(atta::eulerAngles(rotation));
	return atta::vec3();
}

atta::mat4 Object::getModelMat()
{
	// TODO stores current modelMat and calculates if the object has moved
	// (Go up until find updated (world) modelMat)
	//if(_bodyPhysics != nullptr)
	//{
	//	_position = _bodyPhysics->getPosition();
	//	_orientation = _bodyPhysics->getOrientation();
	//	//_rotation = atta::vec3(0,0,0);//_bodyPhysics->getRotation();
	//}

	atta::mat4 mat = atta::mat4(1);

	//if(_parent != nullptr)
	//{
	//	atta::vec3 parentPos = _parent->getPosition();
	//	atta::vec3 parentRot = _parent->getRotation();
	//	mat = atta::translate(mat, parentPos);
	//	mat = atta::rotate(mat, atta::radians(parentRot.z), atta::vec3(0, 0, 1));
	//	mat = atta::rotate(mat, atta::radians(parentRot.y), atta::vec3(0, 1, 0));
	//	mat = atta::rotate(mat, atta::radians(parentRot.x), atta::vec3(1, 0, 0));
	//
	//	std::string constraintType = _parentConstraint->getType();
	//	if(constraintType == "FixedConstraint")
	//	{
	//		FixedConstraint* constraint = (FixedConstraint*)_parentConstraint;
	//		atta::vec3 pos = constraint->getPosition();
	//		atta::vec3 rot = constraint->getRotation();

	//		mat = atta::translate(mat, pos);
	//		mat = atta::rotate(mat, atta::radians(rot.z), atta::vec3(0, 0, 1));
	//		mat = atta::rotate(mat, atta::radians(rot.y), atta::vec3(0, 1, 0));
	//		mat = atta::rotate(mat, atta::radians(rot.x), atta::vec3(1, 0, 0));
	//	}
	//	else if(constraintType == "HingeConstraint")
	//	{
	//		HingeConstraint* constraint = (HingeConstraint*)_parentConstraint;
	//		atta::vec3 pos = constraint->getPosition();
	//		atta::vec3 rot = constraint->getRotation();

	//		mat = atta::translate(mat, pos);
	//		mat = atta::rotate(mat, atta::radians(rot.z), atta::vec3(0, 0, 1));
	//		mat = atta::rotate(mat, atta::radians(rot.y), atta::vec3(0, 1, 0));
	//		mat = atta::rotate(mat, atta::radians(rot.x), atta::vec3(1, 0, 0));
	//	}
	//}

	mat.setPosOriScale(_position, _orientation, _scale);
	mat.transpose();
	//mat = atta::rotate(mat, atta::radians(_rotation.x), atta::vec3(1, 0, 0));
	//mat = atta::rotate(mat, atta::radians(_rotation.y), atta::vec3(0, 1, 0));
	//mat = atta::rotate(mat, atta::radians(_rotation.z), atta::vec3(0, 0, 1));
	//mat = atta::scale(mat, _scale);
	
	return mat;
}

void Object::setPosition(atta::vec3 position)
{
	_position = position;
	//if(_bodyPhysics!=nullptr)
	//	_bodyPhysics->setPosition(atta::vec3(position));
}

void Object::setRotation(atta::vec3 rotation)
{
	for(auto child : _children)
	{
		atta::vec3 rot = rotation - _rotation;
		atta::vec3 childRot = child->getRotation();
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
