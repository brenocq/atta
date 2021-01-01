//--------------------------------------------------
// GuiB
// protect.cpp
// Date: 2020-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "protect.h"

namespace guib
{
	Protect::Protect(ProtectInfo info):
		Widget({}), _protectedChild(info.child)
	{
		Widget::setType("Protect");
		if(Widget::getChild()!=nullptr)
			Widget::setSize(Widget::getChild()->getSize());
	}

	Protect::~Protect()
	{

	}

	void Protect::render()
	{

	}
}
