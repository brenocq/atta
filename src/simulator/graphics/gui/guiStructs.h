//--------------------------------------------------
// Robot Simulator
// guiStructs.h
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H
#include "simulator/math/math.h"
#include <vector>

struct GuiUniformBufferObject
{
	float ratio;
	uint32_t debug;
};

struct GuiObjectInfo
{
	atta::vec2 position = atta::vec2(0.0f, 0.0f);
	atta::vec2 size = atta::vec2(0.5f, 0.5f);
	atta::vec4 color = {1.0f,1.0f,1.0f,1.0f};
	float radius = 0.2f;
	int isLetter = 0;
	atta::vec2 offsetLetter = atta::vec2(0.0f, 0.0f);
	atta::vec2 sizeLetter = atta::vec2(1.0f, 1.0f);
};

#endif// GUI_STRUCTS_H
