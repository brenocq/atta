//--------------------------------------------------
// GuiB
// box.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "box.h"
#include "../guiStructs.h"
#include "../guiState.h"
#include "../guiRender.h"
#include "simulator/helpers/log.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace guib
{
	Box::Box(BoxInfo boxInfo):
		Widget({.offset={0,0}, .size=boxInfo.size, .child=boxInfo.child}), _color(boxInfo.color), _radius(boxInfo.radius)
	{
		Widget::setType("Box");
	}

	Box::~Box()
	{

	}

	void Box::render()
	{
		guib::Color color = getColor();

		GuiObjectInfo objectInfo;
		objectInfo.position = glm::vec2(_offset.x, _offset.y);
		objectInfo.size = glm::vec2(_size.width, _size.height);
		objectInfo.color = glm::vec4(color.r, color.g, color.b, color.a);
		objectInfo.isLetter = 0;

		//Log::debug("Box", "Render [w]$0[] with [w]$1 []--[w] $2", _type, _size.toString(), _offset.toString());

		// Calculate radius
		float minSize = std::min(_size.height, _size.width);
		objectInfo.radius = _radius.topLeft*minSize*2;

		vkCmdPushConstants(
				(state::guiRender)->getCommandBuffer(),
				(state::guiRender)->getPipelineLayout()->handle(),
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(GuiObjectInfo),
				&objectInfo);

		vkCmdDraw((state::guiRender)->getCommandBuffer(), 6, 1, 0, 0);

		if(_child)
			_child->render();
	}
}
