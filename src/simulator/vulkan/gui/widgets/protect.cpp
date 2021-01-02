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
		Widget({.child=info.child})
	{
		Widget::setType("Protect");
	}

	Protect::~Protect()
	{
	}

	void Protect::preProcessSizeOffset()
	{
		Widget::wrapChild();
	}
}
