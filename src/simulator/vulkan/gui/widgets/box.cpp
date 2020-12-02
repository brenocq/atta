//--------------------------------------------------
// GuiB
// box.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "box.h"

guib::Box::Box(BoxInfo boxInfo):
	Widget({.size=boxInfo.size, .child=boxInfo.child}), _color(boxInfo.color), _radius(boxInfo.radius)
{
	Widget::setType("Box");
}

guib::Box::~Box()
{

}
