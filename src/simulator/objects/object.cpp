//--------------------------------------------------
// Robot Simulator
// object.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "object.h"
#include <iostream>

namespace atta
{
	int Object::_qtyIds = 0;
	
	Object::Object(CreateInfo info):
		_type("Object"), _name(info.name), _selection(ObjectSelection::UNSELECTED)
	{
		_id = _qtyIds++;
	}

	Object::~Object()
	{
	}

	//void Object::setSelection(ObjectSelection sel)
	//{
	//	_selection = sel;
	//	for(auto child : _children)
	//		child->setSelection(sel);
	//}

	//void Object::addChild(Object* child, Constraint* constraint)
	//{
	//	// Can't add same child twice
	//	for(auto currChild : _children)
	//		if(currChild == child)
	//			return;

	//	// Change child parent
	//	Object* oldChildParent = child->getParent();
	//	if(oldChildParent != nullptr)
	//	{
	//		oldChildParent->removeChild(child);
	//	}

	//	child->setParent(this);
	//	child->setParentConstraint(constraint);
	//	_children.push_back(child);
	//}

	//void Object::removeChild(Object* child)
	//{
	//	for(int i=0; i<(int)_children.size(); i++)
	//	{
	//		if(_children[i] == child)
	//		{
	//			child->setParent(nullptr);
	//			child->setParentConstraint(nullptr);
	//			_children.erase(_children.begin() + i);
	//			return;
	//		}
	//	}
	//}

	//void Object::setParentConstraint(atta::phy::Constraint* constraint)
	//{
	//	if(_parentConstraint != nullptr)
	//	{
	//		delete _parentConstraint;
	//		_parentConstraint = nullptr;
	//	}

	//	_parentConstraint = constraint;
	//}
}
