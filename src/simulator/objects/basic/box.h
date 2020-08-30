//--------------------------------------------------
// Robot Simulator
// box.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BOX_H
#define BOX_H
#include "../../object.h"
#include "../../vulkan/model.h"

class Box : public Object
{
	public:
		Box(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, glm::vec3 size = {1,1,1}, float mass = 1.0f, glm::vec3 color = {1,1,1});
		~Box();

		//---------- Getters ----------//
		Model* getModel() const { return _model; }

	private:
		Model* _model;
};

#endif// BOX_H
