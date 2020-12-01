//--------------------------------------------------
// GuiB
// clickDetector.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "clickDetector.h"

guib::ClickDetector::ClickDetector(ClickDetectorInfo info):
	Widget({.child=info.child}), _onClick(info.onClick)
{
	Widget::setType("ClickDetector");
}

guib::ClickDetector::~ClickDetector()
{

}
