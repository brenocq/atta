//--------------------------------------------------
// GuiB
// text.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "text.h"
#include "../guiStructs.h"
#include "../guiState.h"
#include "../guiRender.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace guib
{
	Text::Text(TextInfo info):
		Widget({.offset={0,0}, .size={0, info.textSize, UNIT_PERCENT, UNIT_PIXEL}}),
		_color(info.color), _text(info.text), _textSize(info.textSize)
	{
		Widget::setType("Text");
	}

	void Text::preProcessSizeOffset()
	{
		Widget::fillParent();
	}

	void Text::render()
	{
		guib::Color color = _color;

		float currX = _offset.x;
		//std::cout << "Render text: " << text->getText() << std::endl;
		//std::cout << "Size: " << currSize.toString() << std::endl;
		//std::cout << "Offset: " << currOffset.toString() << std::endl;
		for(auto letter : _text)
		{
			
			guib::GlyphInfo gInfo = state::fontLoader->getFontTexture().glyphsInfo[letter];
			float tw = gInfo.width;
			float th = gInfo.height;
			float tx = gInfo.x;
			float ty = gInfo.y;
			float heightPerc = gInfo.height*state::fontLoader->getFontTexture().atlas.height/100.0f;
			float offsetLeft = gInfo.left/gInfo.width;
			float offsetTop = (gInfo.top/gInfo.height);
			float advance = gInfo.advance/gInfo.width;

			//std::cout << "offset " << (char)letter <<" "<< offsetTop << std::endl;

			glm::vec2 textOffset = {currX, _offset.y};
			textOffset.y += _size.height*((1-heightPerc)+(1-offsetTop));

			GuiObjectInfo objectInfo;
			objectInfo.position = textOffset;
			objectInfo.size = glm::vec2(_size.height*(tw/th)*heightPerc, _size.height*heightPerc);
			objectInfo.color = glm::vec4(color.r, color.g, color.b, color.a);
			objectInfo.isLetter = 1;
			objectInfo.offsetLetter = glm::vec2(tx, ty);
			objectInfo.sizeLetter = glm::vec2(tw, th);

			vkCmdPushConstants(
					(state::guiRender)->getCommandBuffer(),
					(state::guiRender)->getPipelineLayout()->handle(),
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
					0,
					sizeof(GuiObjectInfo),
					&objectInfo);

			vkCmdDraw(state::guiRender->getCommandBuffer(), 6, 1, 0, 0);

			currX += _size.height*(tw/th)*heightPerc;
		}
	}
}
