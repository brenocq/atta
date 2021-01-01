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
		Widget({.child=info.child}), 
		_onClick(info.onClick), _onStopClick(info.onStopClick),
		_onRightClick(info.onRightClick), _onStopRightClick(info.onStopRightClick),
		_onHover(info.onHover), _onStopHover(info.onStopHover),
		_hovering(false), _clicking(false), _rightClicking(false)
	{
		Widget::setType("ClickDetector");
		if(Widget::getChild()!=nullptr)
			Widget::setSize(Widget::getChild()->getSize());
	}

	ClickDetector::~ClickDetector()
	{

	}

	void ClickDetector::render()
	{

	}
}
