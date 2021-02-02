//--------------------------------------------------
// GuiB
// menuButton.h
// Date: 2021-02-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_MENU_BUTTON_H
#define GUIB_MENU_BUTTON_H

#include "widget.h"
#include "box.h"
#include <vector>
#include <functional>

namespace guib {
	enum MenuButtonOpen {
		MENU_BUTTON_OPEN_TOP,
		MENU_BUTTON_OPEN_RIGHT,
		MENU_BUTTON_OPEN_BOTTOM,
		MENU_BUTTON_OPEN_LEFT
	};

	struct MenuButtonInfo {
		std::string name = "MenuButton";
		Color color = {.3,.3,.3,1};
		Color hoverColor = {.35,.35,.35,1};
		Color clickColor = {.4,.4,.4,1};
		BoxRadius radius = {.0,.0,.0,.0};

		MenuButtonOpen open = MENU_BUTTON_OPEN_BOTTOM;
		Size buttonSize = {100, 1, UNIT_PIXEL};
		Size menuSize = {.2, .3};
		std::vector<Widget*> children;
	};

	class MenuButton : public Widget
	{
		public:
			MenuButton(MenuButtonInfo info);

		private:
			// MenuButton parameters
			Color _color;
			Color _hoverColor;
			Color _clickColor;
			BoxRadius _radius;

			Box* _boxChild;
	};
}

#endif// GUIB_MENU_BUTTON_H
