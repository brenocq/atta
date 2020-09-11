//--------------------------------------------------
// Robot Simulator
// importedObject.cpp
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "importedObject.h"

ImportedObject::ImportedObject(std::string name, std::string fileName, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float mass):
	Object(name, position, rotation, scale, mass)
{
	_type = "ImportedObject";
	_model = new Model(fileName);
	_physics = new ObjectPhysics(_position, _rotation, mass);
}

ImportedObject::~ImportedObject()
{
	if(_model != nullptr)
	{
		delete _model;
		_model = nullptr;
	}
}
