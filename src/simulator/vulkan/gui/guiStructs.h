//--------------------------------------------------
// Robot Simulator
// guiStructs.h
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H
#include <glm/glm.hpp>
#include <vector>

struct GuiUniformBufferObject
{
	float ratio;
	uint32_t debug;
};

struct GuiObjectInfo
{
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	glm::vec2 size = glm::vec2(0.5f, 0.5f);
	glm::vec4 color = {1.0f,1.0f,1.0f,1.0f};
	float radius = 0.2f;
};

#endif// GUI_STRUCTS_H
