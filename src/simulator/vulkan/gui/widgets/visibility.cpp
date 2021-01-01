//--------------------------------------------------
// GuiB
// visibility.cpp
// Date: 2020-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "visibility.h"

namespace guib
{
	Visibility::Visibility(VisibilityInfo info):
		Widget({.child=info.child}), _visible(info.visible)
	{
		Widget::setType("Visibility");
		if(Widget::getChild()!=nullptr)
			Widget::setSize(Widget::getChild()->getSize());
	}

	Visibility::~Visibility()
	{

	}

	void Visibility::render()
	{

	}
}

