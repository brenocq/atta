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

	void Column::preProcessSizeOffset()
	{
		Widget::fillParent();
	}

	void Column::startPreProcess()
	{
		Widget::startPreProcess();
		for(auto& child : _children)
			child->setParent(this);
	}

	void Column::endPreProcess()
	{
		Widget::endPreProcess();
		Offset columnOffset = _offset;

		Size totalSize = getChildrenTotalSize();
		// Calculate initial offset
		switch(_vAlignment)
		{
			case guib::ALIGN_START:
				break;
			case guib::ALIGN_CENTER:
				{
					float offset = (_size.height-totalSize.height)*0.5f;
					_offset.y += offset;
				}
				break;
			case guib::ALIGN_END:
				{
					float offset = _size.height-totalSize.height;
					_offset.y += offset;
				}
				break;
		}

		for(auto& child : _children)
		{
			Size childSize;
			Offset childOffset;
			child->parentAsksSizeOffset(childSize, childOffset);

			// Add offset of each child
			switch(_hAlignment)
			{
				case guib::ALIGN_START:
					break;
				case guib::ALIGN_CENTER:
					{
						float offset = (_size.width-childSize.width)*0.5f;
						_offset.x = columnOffset.x + offset;
					}
					break;
				case guib::ALIGN_END:
					{
						float offset = _size.width-childSize.width;
						_offset.x = columnOffset.x + offset;
					}
					break;
			}

			// Process next child
			child->treePreProcess();
			
			_offset.y+=childSize.height;	
		}
		_offset = columnOffset;
	}

	void Column::render()
	{
		for(auto& child : _children)
		{
			child->render();
		}
	}

	Size Column::getChildrenTotalSize()
	{
		float maxW = 0;
		float sumH = 0;
		
		for(auto& child : _children)
		{
			Size childSize;
			Offset childOffset;
			child->parentAsksSizeOffset(childSize, childOffset);

			maxW=std::max(maxW, childSize.width);
			sumH+=childSize.height;
		}

		return {maxW, sumH, UNIT_SCREEN, UNIT_SCREEN};
	}

	void Column::addOffsetTree(Offset offset)
	{
		Widget::addOffsetTree(offset);
		for(auto& child : _children)
			child->addOffsetTree(offset);
	}
}
