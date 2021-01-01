//--------------------------------------------------
// GuiB
// row.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "row.h"
#include "../guiState.h"

namespace guib
{
	Row::Row(RowInfo rowInfo):
		Widget({}), _children(rowInfo.children), 
		_hAlignment(rowInfo.hAlignment), _vAlignment(rowInfo.vAlignment)
	{
		Widget::setType("Row");
	}

	Row::~Row()
	{
		for(auto& child : _children)
		{
			if(child!=nullptr)
			{
				delete child;
				child = nullptr;
			}
		}
	}

	void Row::render()
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
				childSize.width /= state::screenSize.width;
				childSize.unitW = guib::UNIT_PERCENT;
			}
			if(childSize.unitH == guib::UNIT_PIXEL)
			{
				childSize.height /= state::screenSize.height;
				childSize.unitH = guib::UNIT_PERCENT;
			}

			sumW+=childSize.width;
			maxH=std::max(maxH, childSize.height);
		}

		return {std::min(sumW, 1.0f), std::min(maxH,1.0f)};
	}
}

