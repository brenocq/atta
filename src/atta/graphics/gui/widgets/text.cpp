//--------------------------------------------------
// GuiB
// text.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/text.h>
#include <atta/graphics/gui/guiStructs.h>
#include <atta/graphics/gui/guiState.h>
#include <atta/graphics/gui/guiRender.h>
#include <atta/graphics/vulkan/vulkan.h>
#include <atta/math/math.h>

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
		float atlasHeight = state::fontLoader->getFontTexture().atlas.height;
		float atlasWidth = state::fontLoader->getFontTexture().atlas.width;
		float atlasPercentToScreenPercentH = atlasHeight/state::screenSize.height;
		float atlasPercentToScreenPercentW = atlasWidth/state::screenSize.width;

		// {currX, currY} is the glyph origin
		float currX = _offset.x;
		float currY = _offset.y;
		for(auto letter : _text)
		{
			guib::GlyphInfo gInfo = state::fontLoader->getFontTexture().glyphsInfo[letter];
			float tw = gInfo.width;
			float th = gInfo.height;
			float tx = gInfo.x;
			float ty = gInfo.y;
			// Height, left, top, advanced calculated in [0,1]^2 screen coordinate
			float heightPercent = gInfo.height*atlasPercentToScreenPercentH;
			float leftPercent = gInfo.left*atlasPercentToScreenPercentW;
			float topPercent = gInfo.top*atlasPercentToScreenPercentH;
			float advancePercent = gInfo.advance*atlasPercentToScreenPercentW;

			// TODO New percents are specified in pixels, need to transform from px to  pt

			//std::cout << "offset " << (char)letter <<" "<< offsetTop << std::endl;

			atta::vec2 textOffset = {currX+leftPercent, currY-topPercent};

			GuiObjectInfo objectInfo;
			objectInfo.position = textOffset;
			objectInfo.size = atta::vec2((tw/th)*heightPercent, heightPercent);
			objectInfo.color = atta::vec4(color.r, color.g, color.b, color.a);
			// Atlas texture position and size (%)
			objectInfo.isLetter = 1;
			objectInfo.offsetLetter = atta::vec2(tx, ty);
			objectInfo.sizeLetter = atta::vec2(tw, th);

			vkCmdPushConstants(
					state::guiRender->getCommandBuffer(),
					state::guiRender->getPipelineLayout()->handle(),
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
					0,
					sizeof(GuiObjectInfo),
					&objectInfo);

			vkCmdDraw(state::guiRender->getCommandBuffer(), 6, 1, 0, 0);

			currX += advancePercent;
		}
	}
}
