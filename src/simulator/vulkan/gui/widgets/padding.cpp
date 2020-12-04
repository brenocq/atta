//--------------------------------------------------
// GuiB
// padding.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "padding.h"

namespace guib
{
	Padding::Padding(PaddingInfo paddingInfo):
		Widget({.child=paddingInfo.child}), _padding(paddingInfo.padding)
	{
		Widget::setType("Padding");
		if(Widget::getChild()!=nullptr)
			Widget::setSize(Widget::getChild()->getSize());
	}

	Padding::~Padding()
	{

	}
}
