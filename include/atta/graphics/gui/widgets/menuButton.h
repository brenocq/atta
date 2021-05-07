//--------------------------------------------------
// GuiB
// menuButton.h
// Date: 2021-02-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_MENU_BUTTON_H
#define GUIB_MENU_BUTTON_H

#include <atta/graphics/gui/widgets/widget.h>
#include <atta/graphics/gui/widgets/box.h>
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
		std::string text = "";
		std::string image = "";
		Color imageColor = {1,1,1,1};
		Color color = {.3,.3,.3,1};
		Color hoverColor = {.35,.35,.35,1};
		Color clickColor = {.4,.4,.4,1};
		BoxRadius radius = {.4,.4,.4,.4};

		MenuButtonOpen open = MENU_BUTTON_OPEN_BOTTOM;
		Size menuSize = {200, 300, UNIT_PIXEL, UNIT_PIXEL};
		std::vector<Widget*> children;
	};

	class MenuButton : public Widget
	{
		public:
			MenuButton(MenuButtonInfo info);

			void preProcessSizeOffset() override;
			void preProcess() override;

		private:
			// MenuButton parameters
			Color _color;
			Color _hoverColor;
			Color _clickColor;
			BoxRadius _radius;
			MenuButtonOpen _menuOpenDirection;

			Box* _menu;
	};
}

#endif// GUIB_MENU_BUTTON_H
