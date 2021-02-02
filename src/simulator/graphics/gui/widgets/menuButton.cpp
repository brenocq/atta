//--------------------------------------------------
// GuiB
// menuButton.cpp
// Date: 2021-02-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "menuButton.h"
#include "button.h"
#include "text.h"

namespace guib
{
	MenuButton::MenuButton(MenuButtonInfo info):
		Widget({}),
		_color(info.color), _hoverColor(info.hoverColor), _clickColor(info.clickColor),
		_radius(info.radius)
	{
		Widget::setType("MenuButton");

		Widget::setChild(new Button(
			{
				.color = info.color,
				.hoverColor = {.35,.35,.35,1},
				.clickColor = {.4,.4,.4,1},
				.onClick = [&](){

				},
				.radius = {.0,.0,.0,.0},
				.size = info.buttonSize,
				.child = new Text(
				{
					.color = {0.8, 0.8, 0.8, 1.0},
					.text = info.name,
					.textSize = 14
				})
			})
		);
	}
}

