//--------------------------------------------------
// GuiB
// padding.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/padding.h>
#include <atta/graphics/gui/guiState.h>

namespace guib
{
	Padding::Padding(PaddingInfo paddingInfo):
		Widget({.child=paddingInfo.child}), _padding(paddingInfo.padding)
	{
		Widget::setType("Padding");

	}

	void Padding::preProcessSizeOffset()
	{
		// Padding implemented as margin
		Widget::fillParent();

		// Convert padding values from pixel to screen
		float invWidth = 1.0f/state::screenSize.width;
		float invHeight = 1.0f/state::screenSize.height;
		if(_padding.unitT == UNIT_PIXEL)
			_padding.top*=invWidth;
		if(_padding.unitR == UNIT_PIXEL)
			_padding.right*=invWidth;
		if(_padding.unitB == UNIT_PIXEL)
			_padding.bottom*=invWidth;
		if(_padding.unitL == UNIT_PIXEL)
			_padding.left*=invWidth;

		// Convert padding values from percent to screen
		if(_padding.unitT == UNIT_PERCENT)
			_padding.top*=_size.height;
		if(_padding.unitR == UNIT_PERCENT)
			_padding.right*=_size.width;
		if(_padding.unitB == UNIT_PERCENT)
			_padding.bottom*=_size.height;
		if(_padding.unitL == UNIT_PERCENT)
			_padding.left*=_size.width;

		// Update offset and size with padding values
		_offset.x	 += _padding.left;
		_offset.y	 += _padding.top;
		_size.width	 -= _padding.left+_padding.right;
		_size.height -= _padding.top+_padding.bottom;
	}
}
