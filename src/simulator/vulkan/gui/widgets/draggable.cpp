//--------------------------------------------------
// GuiB
// draggable.cpp
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "draggable.h"

namespace guib
{
	Draggable::Draggable(DraggableInfo info):
		Widget({.child=info.child}), _active(info.active),
		_dragAreaOffset(info.dragAreaOffset), _dragAreaSize(info.dragAreaSize),
		_widgetToDrag(info.widgetToDrag==nullptr?this:info.widgetToDrag)
	{
		Widget::setType("Draggable");
		if(Widget::getChild()!=nullptr)
			Widget::setSize(Widget::getChild()->getSize());
	}

	Draggable::~Draggable()
	{
	}

	void Draggable::render()
	{

	}
}
