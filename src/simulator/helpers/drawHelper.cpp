//--------------------------------------------------
// Robot Simulator
// drawHelper.cpp
// Date: 2020-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "drawHelper.h"

DrawHelper::DrawHelper()
{

}

DrawHelper::~DrawHelper()
{

}

std::vector<std::pair<glm::vec3, glm::vec3>> DrawHelper::getBoxLines(glm::vec3 size, glm::vec3 position, glm::vec3 rotation)
{
	// TODO return box lines
	std::vector<std::pair<glm::vec3, glm::vec3>> lines;
	lines.push_back(std::make_pair(glm::vec3(0,0,0), glm::vec3(-1, 1, -1)));

	return lines;
}
