//--------------------------------------------------
// GuiB
// topBar.h
// Date: 2021-01-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_TOP_BAR_H
#define GUIB_TOP_BAR_H

#include "widget.h"
#include <vector>

namespace guib {
	struct TopBarInfo {
		Color color = {.15,.15,.15,1};
		std::vector<Widget*> buttons;
	};

	class TopBar : public Widget
	{
		public:
			TopBar(TopBarInfo info);

		private:
			Color _color;
	};
}

#endif// GUIB_TOP_BAR_H
