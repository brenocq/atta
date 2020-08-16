//--------------------------------------------------
// Robot Simulator
// object.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "physics/objectPhysics.h"
#include "vulkan/model.h"

class Object
{
	public:
		Object(std::string name, std::string fileName, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, float mass = 1.0f);
		~Object();

		//---------- Getters ----------//
		Model* getModel() const { return _model; }
		ObjectPhysics* getObjectPhysics() const { return _physics; }
		glm::vec3 getPosition() const { return _position; }
		glm::vec3 getRotation() const { return _rotation; }
		glm::mat4 getModelMat();

	private:
		std::string _name;
		glm::vec3 _position;
		glm::vec3 _rotation;

		ObjectPhysics* _physics;
		Model* _model;
};

#endif// OBJECT_H
