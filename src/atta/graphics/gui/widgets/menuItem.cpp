//--------------------------------------------------
// GuiB
// menuItem.cpp
// Date: 2021-05-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/menuItem.h>
#include <atta/graphics/gui/widgets/widgets.h>
#include <atta/graphics/gui/guiState.h>
#include <atta/graphics/gui/font/fontLoader.h>

namespace guib
{
	MenuItem::MenuItem(MenuItemInfo info):
		Widget((WidgetInfo){.size = calculateButtonSize()})
	{
		Widget::setType("MenuItem");
		// Calculate text maximum width

		Widget::setChild(new Button(
			{
				.color = info.color,
				.hoverColor = info.hoverColor,
				.clickColor = info.clickColor,
				.onClick = info.onClick,
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

	Size MenuItem::calculateButtonSize()
	{
		unsigned textHeight = state::fontLoader->getBaseHeight();
		_paddingV = 3;
		_paddingH = 10;

		return {1,textHeight+_paddingV*2, UNIT_PERCENT, UNIT_PIXEL};
	}
}

