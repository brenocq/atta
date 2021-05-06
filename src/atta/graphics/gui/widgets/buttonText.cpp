//--------------------------------------------------
// GuiB
// buttonText.cpp
// Date: 2021-05-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/buttonText.h>
#include <atta/graphics/gui/widgets/widgets.h>
#include <atta/graphics/gui/guiState.h>
#include <atta/graphics/gui/font/fontLoader.h>

namespace guib
{
	ButtonText::ButtonText(ButtonTextInfo info):
		Widget((WidgetInfo){.size = calculateButtonSize(info.text)})
	{
		Widget::setType("ButtonText");
		// Calculate text maximum width

		Widget::setChild(new Button(
			{
				.color = info.color,
				.hoverColor = info.hoverColor,
				.clickColor = info.clickColor,
				.onClick = info.onClick,
				.radius = info.radius,
				.size = {1,1},// Take full buttonText width and height
				.child = new Text(
				{
					.color = {1.0, 1.0, 1.0, 1.0},
					.text = info.text,
					.textSize = 13,
					.offset = {_paddingH, _paddingV, UNIT_PIXEL, UNIT_PIXEL}
				})
			})
		);
	}

	Size ButtonText::calculateButtonSize(std::string text)
	{
		unsigned textWidth = 0;
		unsigned textSize = state::fontLoader->getBaseHeight();
		_paddingH = 10;
		_paddingV = 3;
		size_t i=0;
		for(auto letter : text)
		{
			textWidth += state::fontLoader->getFontTexture().glyphsInfo[(size_t)letter].advance;
			i++;
		}

		return {textWidth+_paddingH*2,textSize+_paddingV*2, UNIT_PIXEL, UNIT_PIXEL};
	}
}

