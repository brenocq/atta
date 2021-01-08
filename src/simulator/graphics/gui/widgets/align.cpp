//--------------------------------------------------
// GuiB
// align.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "align.h"
#include "simulator/helpers/log.h"
#include <math.h>

namespace guib
{
	Align::Align(AlignInfo info):
		Widget({.child=info.child}),
		_hAlignment(info.hAlignment), _vAlignment(info.vAlignment)
	{
		Widget::setType("Align");
	}

	void Align::preProcessSizeOffset()
	{
		Widget::fillParent();
		if(_child)
		{
			Size childSize;
			Offset childOffset;
			_child->parentAsksSizeOffset(childSize, childOffset);

			switch(_hAlignment)
			{
				case guib::ALIGN_START:
					{
					}
					break;
				case guib::ALIGN_CENTER:
					{
						float offset = (_size.width-childSize.width)*0.5f;
						_offset.x = _offset.x+offset;
					}
					break;
				case guib::ALIGN_END:
					{
						float offset = _size.width-childSize.width;
						_offset.x = _offset.x+offset;
					}
					break;
			}

			switch(_vAlignment)
			{
				case guib::ALIGN_START:
					{
					}
					break;
				case guib::ALIGN_CENTER:
					{
						float offset = (_size.height-childSize.height)*0.5f;
						_offset.y = _offset.y+offset;
					}
					break;
				case guib::ALIGN_END:
					{
						float offset = _size.height-childSize.height;
						_offset.y = _offset.y+offset;
					}
					break;
			}

			_size = childSize;
		}
	}
}
