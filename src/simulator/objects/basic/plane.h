//--------------------------------------------------
// Robot Simulator
// plane.h
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PLANE_H
#define PLANE_H
#include "../../object.h"

class Plane : public Object
{
	public:
		Plane(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, glm::vec2 size = {1,1}, float mass = 1.0f, glm::vec3 color = {1,1,1});
		~Plane();

		//---------- Getters ----------//
		glm::vec3 getColor() const { return _color; };

	private:
		glm::vec3 _color;
		glm::vec2 _size;
};

#endif// PLANE_H
