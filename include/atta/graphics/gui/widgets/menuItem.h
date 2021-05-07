//--------------------------------------------------
// GuiB
// menuItem.h
// Date: 2021-05-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_MENU_ITEM_H
#define GUIB_MENU_ITEM_H

#include <atta/graphics/gui/widgets/widget.h>
#include <atta/graphics/gui/widgets/box.h>
#include <vector>
#include <functional>

namespace guib {
	struct MenuItemInfo {
		std::string text = "Button";
		Color color = {0,0,0,0};
		Color hoverColor = {.2,.2,.2,1};
		Color clickColor = {.3,.3,.3,1};
		std::function<void()> onClick;
	};

	class MenuItem : public Widget
	{
		public:
			MenuItem(MenuItemInfo info);

		private:
			Size calculateButtonSize();
			unsigned _paddingH;
			unsigned _paddingV;
	};
}

#endif// GUIB_MENU_ITEM_H
