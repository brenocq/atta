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
#include "simulator/helpers/log.h"

Ttzinho::Ttzinho()
{
	_object = new Box("Ttzinho", {0,0.06,0}, {0,0,0}, {0.18, 0.01,0.05}, 0.1f, {0,0,0.8});
	ImportedObject* wheelL = new ImportedObject("Wheel left", "wheel", {0.2,0.06,0}, {0,0,90}, {1,1,1}, 0.1f, new btCylinderShape(btVector3(0.06, 0.05, 0.06)));
	ImportedObject* wheelR = new ImportedObject("Wheel right", "wheel", {-0.2,0.06,0}, {0,0,90}, {1,1,1}, 0.1f, new btCylinderShape(btVector3(0.06, 0.05, 0.06)));

	_object->addChild(wheelL, new FixedConstraint({0.2,0,0}, {0,0,0}));
	_object->addChild(wheelR, new FixedConstraint({-0.2,0,0}, {0,0,0}));

	Log::debug("Ttzinho", "Created!");
	Log::debug("Ttzinho", "Created!", false);
	Log::debug("Ttzinho", "Created!", false);
	Log::info("Ttzinho", "Created!");
	Log::info("Ttzinho", "Created!", false);
	Log::info("Ttzinho", "Created!", false);
	Log::warning("Ttzinho", "Created!");
	Log::success("Ttzinho", "Created!");
	Log::error("Ttzinho", "Created!");
}

Ttzinho::~Ttzinho()
{
	if(_object != nullptr)
	{
		delete _object;
		_object = nullptr;
	}

}
