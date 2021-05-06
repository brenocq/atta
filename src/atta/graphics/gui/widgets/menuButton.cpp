//--------------------------------------------------
// GuiB
// menuButton.cpp
// Date: 2021-02-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/menuButton.h>
#include <atta/graphics/gui/widgets/buttonText.h>
#include <atta/graphics/gui/widgets/buttonImage.h>
#include <atta/graphics/gui/widgets/column.h>
#include <atta/graphics/gui/guiState.h>

namespace guib
{
	MenuButton::MenuButton(MenuButtonInfo info):
		Widget({}),
		_color(info.color), _hoverColor(info.hoverColor), _clickColor(info.clickColor),
		_radius(info.radius)
	{
		Widget::setType("MenuButton");

		Color menuColor = state::palette["background"];
		menuColor.a = 0.8;
		_menu = new Box(
					(BoxInfo){
						.color = menuColor,
						.offset = {0, 0, UNIT_PIXEL, UNIT_PIXEL},
						.size = {1000, 100, UNIT_PIXEL, UNIT_PIXEL}
					});

		Widget* button=nullptr;

		if(info.text!="")
		{
			button = new ButtonText(
				{
					.text = info.text,
					.color = info.color,
					.hoverColor = info.hoverColor,
					.clickColor = info.clickColor,
					.onClick = [&](){
						if(state::focusedWidget == (Widget*)_menu)
							state::focusedWidget = nullptr;
						else
							state::focusedWidget = (Widget*)_menu;
					},
					.radius = info.radius,
				});
		}
		else if(info.image != "")
		{
			button = new ButtonImage(
				{
					.image = info.image,
					.imageColor = info.imageColor,
					.color = info.color,
					.hoverColor = info.hoverColor,
					.clickColor = info.clickColor,
					.onClick = [&](){
						if(state::focusedWidget == (Widget*)_menu)
							state::focusedWidget = nullptr;
						else
							state::focusedWidget = (Widget*)_menu;
					},
					.radius = info.radius,
				});
		}

		Widget::setChild(button);
	}

	void MenuButton::preProcessSizeOffset()
	{
		Widget::wrapChild();
		_menu->preProcessSizeOffset();// Solve menu size -> convert from pixel to screen coordinate system
	}
}

