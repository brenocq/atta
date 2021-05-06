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
		Widget({.offset=info.offset, .size=calculateTextSize(info.text, info.textSize)}),
		_color(info.color), _text(info.text), _textSize(info.textSize)
	{
		Widget::setType("Text");
	}

	//void Text::preProcessSizeOffset()
	//{
	//	//Widget::fillParent();
	//}

	void Text::render()
	{
		guib::Color color = _color;
		float atlasHeight = state::fontLoader->getFontTexture().atlas.height;
		float atlasWidth = state::fontLoader->getFontTexture().atlas.width;
		float sizeTransform = _textSize/state::fontLoader->getBaseHeight();// Scale glyph pixels to desired pixel size

		// {currX, currY} is the glyph origin
		float currX = _offset.x;
		float currY = _offset.y;
		currY += _size.height/2.0f+_textSize*0.75f/float(state::screenSize.height*2);// Center vertically

		float totalWidth=0;
		for(auto letter : _text)
		{
			guib::GlyphInfo gInfo = state::fontLoader->getFontTexture().glyphsInfo[letter];
			totalWidth += gInfo.width/state::screenSize.width;
		}
		//currX += _size.width/2.0f-totalWidth/2.0f;// Center horizontally
		
		for(auto letter : _text)
		{
			guib::GlyphInfo gInfo = state::fontLoader->getFontTexture().glyphsInfo[letter];
			double tw = gInfo.width/atlasWidth;
			double th = gInfo.height/atlasHeight;
			double tx = gInfo.x/atlasWidth;
			double ty = gInfo.y/atlasHeight;
			// Height, left, top, advanced calculated in [0,1]^2 screen coordinate
			double heightPercent = gInfo.height/state::screenSize.height;
			double leftPercent = gInfo.left/state::screenSize.width;
			double topPercent = gInfo.top/state::screenSize.height;
			double advancePercent = gInfo.advance/state::screenSize.width;

			heightPercent *= sizeTransform;
			leftPercent *= sizeTransform;
			topPercent *= sizeTransform;
			advancePercent *= sizeTransform;

			if(letter==' ')
			{
				currX += advancePercent;
				continue;
			}

			atta::vec2 textOffset = {currX+leftPercent, currY-topPercent};

			GuiObjectInfo objectInfo;
			objectInfo.position = atta::vec4(textOffset, (float)state::renderDepth, 1.0f);
			float screenRatio = state::screenSize.height/(float)state::screenSize.width;
			objectInfo.size = atta::vec2((gInfo.width/(float)gInfo.height)*heightPercent*screenRatio, heightPercent);
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

			//currX += (gInfo.width/gInfo.height)*heightPercent;
			currX += advancePercent;
		}
	}

	Size Text::calculateTextSize(std::string text, unsigned textSize)
	{
		unsigned textWidth = 0;
		for(auto letter : text)
		{
			textWidth += state::fontLoader->getFontTexture().glyphsInfo[(size_t)letter].advance;
		}

		return {textWidth, textSize, UNIT_PIXEL, UNIT_PIXEL};
	}
}
