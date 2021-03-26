//--------------------------------------------------
// GuiB
// clickDetector.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/clickDetector.h>
#include <atta/graphics/gui/guiState.h>

namespace guib
{
	ClickDetector::ClickDetector(ClickDetectorInfo info):
		Widget({.child=info.child}), 
		_onClick(info.onClick), _onStopClick(info.onStopClick),
		_onRightClick(info.onRightClick), _onStopRightClick(info.onStopRightClick),
		_onHover(info.onHover), _onStopHover(info.onStopHover),
		_hovering(false), _clicking(false), _rightClicking(false),
		_clickableStateIndex(-1)
	{
		Widget::setType("ClickDetector");
	}


	void ClickDetector::preProcessSizeOffset()
	{
		Widget::wrapChild();
	}

	void ClickDetector::preProcess()
	{
		_clickableStateIndex = state::clickableAreas.size();
		state::clickableAreas.push_back({_offset, _size, this});
	}

	void ClickDetector::addOffsetTree(Offset offset)
	{
		state::clickableAreas[_clickableStateIndex].offset += offset;
		Widget::addOffsetTree(offset);
	}
}
