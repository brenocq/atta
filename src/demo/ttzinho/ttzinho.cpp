//--------------------------------------------------
// Robot Ttzinho
// ttzinho.cpp
// Date: 2020-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "ttzinho.h"
#include "simulator/objects/basic/box.h"
#include "simulator/objects/basic/importedObject.h"
#include "simulator/physics/constraints/fixedConstraint.h"

Ttzinho::Ttzinho()
{
	_object = new Box("Ttzinho", {0,0.06,0}, {0,0,0}, {0.18, 0.01,0.05}, 0.0f, {0,0,0.8});
	ImportedObject* wheelL = new ImportedObject("Wheel left", "wheel", {0.1,0.06,0}, {0,0,90}, {1,1,1}, 0.0f);
	ImportedObject* wheelR = new ImportedObject("Wheel right", "wheel", {-0.1,0.06,0}, {0,0,90}, {1,1,1}, 0.0f);

	_object->addChild(wheelL, new FixedConstraint({1,0.06,0}, {0,0,90}));
	_object->addChild(wheelR, new FixedConstraint({-1,0.06,0}, {0,0,90}));
}

Ttzinho::~Ttzinho()
{
	if(_object != nullptr)
	{
		delete _object;
		_object = nullptr;
	}

}
