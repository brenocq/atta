//--------------------------------------------------
// GuiB
// column.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "column.h"

namespace guib
{
	Column::Column(ColumnInfo columnInfo):
		Widget({}), _children(columnInfo.children), 
		_vAlignment(columnInfo.vAlignment), _hAlignment(columnInfo.hAlignment)
	{
		Widget::setType("Column");
	}

	Column::~Column()
	{
		for(auto& c : _children)
		{
			if(c!=nullptr)
			{
				delete c;
				c = nullptr;
			}
		}
	}

	Size Column::getChildrenTotalSize()
	{
		float sumH = 0;
		float maxW = 0;
		
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

			sumH+=childSize.height;
			maxW=std::max(maxW, childSize.width);
		}

		return {std::min(maxW, 1.0f), std::min(sumH,1.0f)};
	}
}
