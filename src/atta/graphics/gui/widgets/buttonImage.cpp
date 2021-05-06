//--------------------------------------------------
// GuiB
// buttonImage.cpp
// Date: 2021-05-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/buttonImage.h>
#include <atta/graphics/gui/widgets/button.h>
#include <atta/graphics/gui/widgets/image.h>
#include <atta/graphics/gui/guiState.h>

namespace guib
{
	ButtonImage::ButtonImage(ButtonImageInfo info):
		Widget((WidgetInfo){.size = {20, 20, UNIT_PIXEL, UNIT_PIXEL}})
	{
		Widget::setType("ButtonImage");
		// Calculate text maximum width

		Widget::setChild(new Button(
			{
				.color = info.color,
				.hoverColor = info.hoverColor,
				.clickColor = info.clickColor,
				.onClick = info.onClick,
				.radius = info.radius,
				.size = {1,1},// Take full buttonImage width and height
				.child = new Image(
				{
					.name = info.image,
					.color = info.imageColor,
					.size = {.8,.8},
					.offset = {.1, .1}
				})
			})
		);
	}
}

