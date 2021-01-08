//--------------------------------------------------
// GuiB
// draggable.cpp
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "draggable.h"
#include "../guiState.h"

namespace guib
{
	Draggable::Draggable(DraggableInfo info):
		Widget({.child=info.child}), _active(info.active),
		_widgetToHover(info.widgetToHover==nullptr?this:info.widgetToHover),
		_widgetToDrag(info.widgetToDrag==nullptr?this:info.widgetToDrag),
		_draggableStateIndex(-1)
	{
		Widget::setType("Draggable");
	}

	void Draggable::preProcessSizeOffset()
	{
		Widget::wrapChild();
	}

	void Draggable::preProcess()
	{
		state::draggables.push_back(this);
	}
}
