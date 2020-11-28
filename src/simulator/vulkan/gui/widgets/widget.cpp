//--------------------------------------------------
// GuiB
// widget.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "widget.h"

guib::Widget::Widget(WidgetInfo widgetInfo):
	_size(widgetInfo.size), _offset(widgetInfo.offset), _child(widgetInfo.child), _type("Widget")
{

}

guib::Widget::~Widget()
{
	if(_child!=nullptr)
	{
		delete _child;
		_child = nullptr;
	}
}
