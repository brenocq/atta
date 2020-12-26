//--------------------------------------------------
// Robot Simulator
// capsule.h
// Date: 2020-12-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CAPSULE_H
#define CAPSULE_H
#include "../../object.h"
#include "../../vulkan/model.h"

class Capsule : public Object
{
	public:
		Capsule(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, glm::vec3 scale = {0,0,0}, float mass = 1.0f, glm::vec3 color = {1,1,1});
		~Capsule();

		//---------- Getters ----------//
		glm::vec3 getColor() const { return _color; };

	private:
		glm::vec3 _color;

};

#endif// CAPSULE_H
