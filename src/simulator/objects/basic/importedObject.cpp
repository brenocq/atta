//--------------------------------------------------
// Robot Simulator
// importedObject.cpp
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "importedObject.h"

ImportedObject::ImportedObject(std::string name, std::string fileName, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float mass, btCollisionShape* collisionShape):
	Object(name, position, rotation, scale, mass)
{
	_type = "ImportedObject";
	_model = new Model(fileName);
	if(collisionShape == nullptr)
		// TODO calculate convex hull shape
		_physics = new ObjectPhysics(new btBoxShape(btVector3(0.5,0.5,0.5)), _position, _rotation, mass);
	else
		_physics = new ObjectPhysics(collisionShape, _position, _rotation, mass);
}

ImportedObject::~ImportedObject()
{
	if(_model != nullptr)
	{
		delete _model;
		_model = nullptr;
	}
}
