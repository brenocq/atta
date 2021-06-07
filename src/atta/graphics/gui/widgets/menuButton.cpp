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
#include <atta/helpers/log.h>

namespace guib
{
	MenuButton::MenuButton(MenuButtonInfo info):
		Widget({}),
		_color(info.color), _hoverColor(info.hoverColor), _clickColor(info.clickColor),
		_radius(info.radius), _menuOpenDirection(info.open)
	{
		Widget::setType("MenuButton");

		Color menuColor = state::palette["background"];
		menuColor.a = 0.97;
		_menu = new Box(
					(BoxInfo){
						.color = menuColor,
						.offset = {0, 0, UNIT_PIXEL, UNIT_PIXEL},
						.size = info.menuSize,
						.child = new Column(
							{
								.children = info.children
							})
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
					.size = {20, 20, guib::UNIT_PIXEL, guib::UNIT_PIXEL},
					.radius = info.radius,
				});
		}

		Widget::setChild(button);
	}

	void MenuButton::preProcessSizeOffset()
	{
		// Wrap child size/offset
		Widget::wrapChild();
		_menu->preProcessSizeOffset();// Solve menu size -> convert from pixel to screen coordinate system
	}

	void MenuButton::preProcess()
	{
		//---------- Update menu offset from menu button position ----------//
		// Here we are also preprocessing the menu and its children
		_menu->startPreProcess();
		{
			// Size already convertes to UNIT_SCREEN
			Size menuSize = _menu->getSize();

			// Calculate menu offset
			switch(_menuOpenDirection)
			{
				case MENU_BUTTON_OPEN_TOP:
					_menu->setOffset({_offset.x, _offset.y-menuSize.height, UNIT_SCREEN, UNIT_SCREEN});
				break;
				case MENU_BUTTON_OPEN_RIGHT:
					_menu->setOffset({_offset.x+_size.width, _offset.y, UNIT_SCREEN, UNIT_SCREEN});
				break;
				case MENU_BUTTON_OPEN_BOTTOM:
					_menu->setOffset({_offset.x, _offset.y+_size.height, UNIT_SCREEN, UNIT_SCREEN});
				break;
				case MENU_BUTTON_OPEN_LEFT:
					_menu->setOffset({_offset.x-menuSize.width, _offset.y, UNIT_SCREEN, UNIT_SCREEN});
				default:
				break;
			}
		}
		// Preprocess menu tree
		_menu->preProcess();
		_menu->endPreProcess();
	}
}

