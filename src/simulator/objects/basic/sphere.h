//--------------------------------------------------
// Robot Simulator
// sphere.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SPHERE_H
#define SPHERE_H
#include "../../object.h"
#include "../../vulkan/model.h"

class Sphere : public Object
{
	public:
		Sphere(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, float radius = 0.5f, float mass = 1.0f, glm::vec3 color = {1,1,1});
		~Sphere();

		//---------- Getters ----------//
		Model* getModel() const { return _model; }

	private:
		Model* _model;
};

#endif// SPHERE_H
