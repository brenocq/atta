//--------------------------------------------------
// GuiB
// row.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "row.h"
#include "../guiState.h"
#include "simulator/helpers/log.h"

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

	void Row::preProcessSizeOffset()
	{
		Widget::fillParent();
	}

	void Row::startPreProcess()
	{
		Widget::startPreProcess();
		for(auto& child : _children)
			child->setParent(this);
	}

	void Row::endPreProcess()
	{
		Widget::endPreProcess();
		Offset rowOffset = _offset;

		Size totalSize = getChildrenTotalSize();
		// Calculate initial offset
		switch(_hAlignment)
		{
			case guib::ALIGN_START:
				break;
			case guib::ALIGN_CENTER:
				{
					float offset = (_size.width-totalSize.width)*0.5f;
					_offset.x += offset;
				}
				break;
			case guib::ALIGN_END:
				{
					float offset = _size.width-totalSize.width;
					_offset.x += offset;
				}
				break;
		}

		for(auto& child : _children)
		{
			Size childSize;
			Offset childOffset;
			child->parentAsksSizeOffset(childSize, childOffset);

			// Add offset of each child
			switch(_vAlignment)
			{
				case guib::ALIGN_START:
					break;
				case guib::ALIGN_CENTER:
					{
						float offset = (_size.height-childSize.height)*0.5f;
						_offset.y = rowOffset.y + offset;
					}
					break;
				case guib::ALIGN_END:
					{
						float offset = _size.height-childSize.height;
						_offset.y = rowOffset.y + offset;
					}
					break;
			}

			// Process next child
			child->treePreProcess();
			
			_offset.x+=childSize.width;	
		}
		_offset = rowOffset;
	}

	void Row::render()
	{
		for(auto& child : _children)
		{
			child->render();
		}
	}

	Size Row::getChildrenTotalSize()
	{
		float maxH = 0;
		float sumW = 0;
		
		for(auto& child : _children)
		{
			Size childSize;
			Offset childOffset;
			child->parentAsksSizeOffset(childSize, childOffset);

			sumW+=childSize.width;
			maxH=std::max(maxH, childSize.height);
		}

		return {sumW, maxH, UNIT_SCREEN, UNIT_SCREEN};
	}

	void Row::addOffsetTree(Offset offset)
	{
		Widget::addOffsetTree(offset);
		for(auto& child : _children)
			child->addOffsetTree(offset);
	}
}

