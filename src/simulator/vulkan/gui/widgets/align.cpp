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

	Align::~Align()
	{
	}

	void Align::preProcess()
	{
		Widget::startPreProcess();
		{
			if(_child)
			{
				Size childSize;
				Offset childOffset;
				_child->getSizeOffset(childSize, childOffset);

				if(std::isnan(childSize.width) || std::isnan(childSize.height) || std::isnan(childOffset.x) || std::isnan(childOffset.y))
				{
					Log::error("guib::Align", "Child size/offset must be possible to calculate!");
					return;
				}

				switch(_hAlignment)
				{
					case guib::ALIGN_START:
						{
							childOffset.x = _offset.x;
						}
						break;
					case guib::ALIGN_CENTER:
						{
							float offset = (_size.width-childSize.width)*0.5f;
							childOffset.x = _offset.x+offset;
						}
						break;
					case guib::ALIGN_END:
						{
							float offset = _size.width-childSize.width;
							childOffset.x = _offset.x+offset;
						}
						break;
				}

				switch(_vAlignment)
				{
					case guib::ALIGN_START:
						{
							childOffset.y = _offset.y;
						}
						break;
					case guib::ALIGN_CENTER:
						{
							float offset = (_size.height-childSize.height)*0.5f;
							childOffset.y = _offset.y+offset;
						}
						break;
					case guib::ALIGN_END:
						{
							float offset = _size.height-childSize.height;
							childOffset.y = _offset.y+offset;
						}
						break;
				}
				_child->setSize(childSize);
				_child->setOffset(childOffset);
			}

		}
		Widget::endPreProcess();
	}
}
