//--------------------------------------------------
// GuiB
// row.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "row.h"

namespace guib
{
	Row::Row(RowInfo rowInfo):
		Widget({}), _children(rowInfo.children), 
		_vAlignment(rowInfo.vAlignment), _hAlignment(rowInfo.hAlignment)
	{
		Widget::setType("Row");
	}

	Row::~Row()
	{

	}

	Size Row::getChildrenTotalSize()
	{
		float maxH = 0;
		float sumW = 0;
		
		for(auto child : _children)
		{
			Size childSize = child->getSize();
			if(childSize.unitW == guib::UNIT_PIXEL)
			{
				childSize.width /= Widget::screenSize.width;
				childSize.unitW = guib::UNIT_PERCENT;
			}
			if(childSize.unitH == guib::UNIT_PIXEL)
			{
				childSize.height /= Widget::screenSize.height;
				childSize.unitH = guib::UNIT_PERCENT;
			}

			sumW+=childSize.width;
			maxH=std::max(maxH, childSize.height);
		}

		return {sumW, maxH};
	}
}

