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
	_bodyPhysics = new Body(&_position, &_orientation, mass);
}

ImportedObject::~ImportedObject()
{
}
