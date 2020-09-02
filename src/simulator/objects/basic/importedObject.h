//--------------------------------------------------
// Robot Simulator
// importedObject.h
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef IMPORTED_OBJECT_H
#define IMPORTED_OBJECT_H

#include "../../object.h"
#include "../../vulkan/model.h"

class ImportedObject : public Object
{
	public:
		ImportedObject(
				std::string name, std::string fileName, 
				glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, glm::vec3 scale = {0,0,0}, 
				float mass = 1.0f,
				btCollisionShape* collisionShape = nullptr);
		~ImportedObject();

		//---------- Getters ----------//
		Model* getModel() const { return _model; }

	private:
		Model* _model;
};

#endif// IMPORTED_OBJECT_H
