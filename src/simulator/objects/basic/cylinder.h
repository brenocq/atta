//--------------------------------------------------
// Robot Simulator
// cylinder.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CYLINDER_H
#define CYLINDER_H
#include "../../object.h"
#include "../../vulkan/model.h"

class Cylinder : public Object
{
	public:
		Cylinder(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, glm::vec3 scale = {0,0,0}, float mass = 1.0f, glm::vec3 color = {1,1,1});
		~Cylinder();

		//---------- Getters ----------//
		Model* getModel() const { return _model; }

	private:
		Model* _model;
};

#endif// CYLINDER_H
