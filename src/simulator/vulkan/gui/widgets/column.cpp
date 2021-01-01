//--------------------------------------------------
// GuiB
// column.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "column.h"
#include "../guiState.h"

namespace guib
{
	Column::Column(ColumnInfo columnInfo):
		Widget({}), _children(columnInfo.children), 
		_hAlignment(columnInfo.hAlignment), _vAlignment(columnInfo.vAlignment)
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

	void Column::render()
	{

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
				childSize.width /= state::screenSize.width;
				childSize.unitW = guib::UNIT_PERCENT;
			}
			if(childSize.unitH == guib::UNIT_PIXEL)
			{
				childSize.height /= state::screenSize.height;
				childSize.unitH = guib::UNIT_PERCENT;
			}

			sumH+=childSize.height;
			maxW=std::max(maxW, childSize.width);
		}

		return {std::min(maxW, 1.0f), std::min(sumH,1.0f)};
	}
}
