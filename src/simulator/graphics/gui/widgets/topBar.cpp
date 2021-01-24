//--------------------------------------------------
// GuiB
// topBar.cpp
// Date: 2021-01-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "topBar.h"
#include "widgets.h"
#include "simulator/helpers/log.h"

namespace guib
{
	TopBar::TopBar(TopBarInfo info):
		Widget({
				.offset = {0,0}, 
				.size={1,20, guib::UNIT_PERCENT, guib::UNIT_PIXEL}}),
		_color(info.color)
	{
		Widget::setType("TopBar");
		updateTree();
	}

	void TopBar::updateTree()
	{
		Widget::setChild(
			new guib::Box(
			{
				.color = _color,
				.size  = {1, 1}
			})
		);
	}
}

