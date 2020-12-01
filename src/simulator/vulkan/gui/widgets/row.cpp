//--------------------------------------------------
// GuiB
// row.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "row.h"
#include <iostream>

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

	Size Row::getChildrenTotalSize(Size currSize)
	{
		float maxH = 0;
		float sumW = 0;
		
		for(auto child : _children)
		{
			Size childSize = child->getSize();
			std::cout << "bef:"<<childSize.width<<" sc:"<<Widget::screenSize.width<<std::endl;
			if(childSize.unitW == guib::UNIT_PIXEL)
			{
				childSize.width /= Widget::screenSize.width*currSize.width;
				childSize.unitW = guib::UNIT_PERCENT;
			}
			if(childSize.unitH == guib::UNIT_PIXEL)
			{
				childSize.height /= Widget::screenSize.height*currSize.height;
				childSize.unitH = guib::UNIT_PERCENT;
			}

			std::cout << "sum:"<<childSize.width<<std::endl;
			sumW+=childSize.width;
			maxH=std::max(maxH, childSize.height);
		}

		return {sumW, maxH};
	}
}

