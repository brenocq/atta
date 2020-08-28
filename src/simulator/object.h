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

class Object
{
	public:
		Object(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, glm::vec3 scale = {1,1,1}, float mass = 1.0f);
		~Object();

		//---------- Getters ----------//
		ObjectPhysics* getObjectPhysics() const { return _physics; }
		glm::vec3 getPosition() const { return _position; }
		glm::vec3 getRotation() const { return _rotation; }
		glm::mat4 getModelMat();
		std::string getType() const { return _type; }
		std::string getName() const { return _name; }

	protected:
		std::string _type;
		std::string _name;
		glm::vec3 _position;
		glm::vec3 _rotation;
		glm::vec3 _scale;
		float _mass;

		ObjectPhysics* _physics;
};

#endif// OBJECT_H
