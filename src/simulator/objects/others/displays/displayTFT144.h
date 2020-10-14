//--------------------------------------------------
// Robot Simulator
// displayTFT144.h
// Date: 2020-10-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DISPLAY_TFT_144_H
#define DISPLAY_TFT_144_H

#include <string>
#include <vector>
#include "simulator/object.h"

class DisplayTFT144 : public Object
{
	public:
		DisplayTFT144(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0});
		~DisplayTFT144();

	private:
};

#endif// DISPLAY_TFT_144_H

