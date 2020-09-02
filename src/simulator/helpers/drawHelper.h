//--------------------------------------------------
// Robot Simulator
// drawHelper.h
// Date: 2020-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DRAW_HELPER_H
#define DRAW_HELPER_H

#include <vector>
#include "glm.h"

class DrawHelper
{
	public:
		DrawHelper();
		~DrawHelper();

		static std::vector<std::pair<glm::vec3, glm::vec3>> getBoxLines(glm::vec3 size, glm::vec3 position, glm::vec3 rotation);

	private:
};

#endif// DRAW_HELPER_H
