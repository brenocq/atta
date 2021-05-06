//--------------------------------------------------
// Robot Simulator
// guiStructs.h
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H
#include <atta/math/math.h>
#include <vector>

struct GuiUniformBufferObject
{
	float ratio;
	uint32_t debug;
};

struct GuiObjectInfo
{
	alignas(16) atta::vec4 position = atta::vec4(0.0f, 0.0f, 0.99f, 1.0f);
	alignas(8) atta::vec2 size = atta::vec2(0.5f, 0.5f);
	alignas(16) atta::vec4 color = {1.0f,1.0f,1.0f,1.0f};
	alignas(4) float radius = 0.2f;
	alignas(4) int isLetter = 0;
	alignas(4) int textureIndex = -1;
	alignas(8) atta::vec2 offsetLetter = atta::vec2(0.0f, 0.0f);
	alignas(8) atta::vec2 sizeLetter = atta::vec2(1.0f, 1.0f);
};

#endif// GUI_STRUCTS_H
