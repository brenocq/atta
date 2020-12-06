//--------------------------------------------------
// GuiB
// align.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "align.h"

namespace guib
{
	Align::Align(AlignInfo info):
		Widget({.child=info.child}),
		_hAlignment(info.hAlignment), _vAlignment(info.vAlignment)
	{
		Widget::setType("Align");
	}

	Align::~Align()
	{
	}
}
