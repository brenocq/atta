//--------------------------------------------------
// GuiB
// image.cpp
// Date: 2021-05-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/image.h>
#include <atta/graphics/gui/guiStructs.h>
#include <atta/graphics/gui/guiState.h>
#include <atta/graphics/gui/guiRender.h>
#include <atta/helpers/log.h>
#include <atta/graphics/vulkan/vulkan.h>
#include <atta/math/math.h>

namespace guib
{
	Image::Image(ImageInfo info):
		Widget({.offset=info.offset, .size=info.size, .child=info.child}), _color(info.color), _radius(info.radius), _name(info.name)
	{
		Widget::setType("Image");
	}

	void Image::render()
	{
		guib::Color color = getColor();

		GuiObjectInfo objectInfo;
		objectInfo.position = atta::vec4(_offset.x, _offset.y, (float)state::renderDepth, 1.0f);
		objectInfo.size = atta::vec2(_size.width, _size.height);
		objectInfo.color = atta::vec4(color.r, color.g, color.b, color.a);
		objectInfo.isLetter = 0;
		objectInfo.textureIndex = _name!="" ? state::textureIndex[_name]+1 : -1;

		// Calculate radius
		float minSize = std::min(_size.height, _size.width);
		objectInfo.radius = _radius.topLeft*minSize/2.0f;

		//Log::debug("Image", "Render texture [w]$0[] with [w]$1 []--[w] $2 and radius:$3", _name, _size.toString(), _offset.toString(), objectInfo.radius);

		vkCmdPushConstants(
				state::guiRender->getCommandBuffer(),
				state::guiRender->getPipelineLayout()->handle(),
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(GuiObjectInfo),
				&objectInfo);

		vkCmdDraw(state::guiRender->getCommandBuffer(), 6, 1, 0, 0);

		if(_child)
			_child->render();
	}
}
