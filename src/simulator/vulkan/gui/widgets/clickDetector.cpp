//--------------------------------------------------
// GuiB
// clickDetector.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "clickDetector.h"

namespace guib
{
	ClickDetector::ClickDetector(ClickDetectorInfo info):
		Widget({.child=info.child}), _onClick(info.onClick)
	{
		Widget::setType("ClickDetector");
		if(Widget::getChild()!=nullptr)
			Widget::setSize(Widget::getChild()->getSize());
	}

	ClickDetector::~ClickDetector()
	{

	}
}