//--------------------------------------------------
// GuiB
// widget.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/widget.h>
#include <atta/graphics/gui/guiState.h>
#include <atta/helpers/log.h>
#include <math.h>
#include <cmath>

namespace guib
{
	Widget::Widget(WidgetInfo widgetInfo):
		_size(widgetInfo.size), _offset(widgetInfo.offset), _child(widgetInfo.child), _parent(nullptr), _type("Widget"),
		_widgetInfo(widgetInfo)
	{
	}

	Widget::~Widget()
	{
		if(_child!=nullptr)
		{
			delete _child;
			_child = nullptr;
		}
	}

	void Widget::render()
	{
		//	Log::debug("Widget", "Render [c]$0 [] Off:$1", _type, _offset.toString());
		if(_child)
			_child->render();
	}


	//--------------------------------//
	//--------- Size/Offset ----------//
	//--------------------------------//
	void Widget::getParentSizeOffset(Size &pSize, Offset &pOffset)
	{
		pSize = {1,1, UNIT_SCREEN, UNIT_SCREEN};
		pOffset = {0,0, UNIT_SCREEN, UNIT_SCREEN};
		if(_parent)
		{
			pSize = _parent->getSize();
			pOffset = _parent->getOffset();
		}
	}

	void Widget::parentAsksSizeOffset(Size &size, Offset &offset)
	{
		// The parent can call this method if it needs to now the child size/offset to define its own
		startPreProcess();
		preProcessSizeOffset();
		size = _size;
		offset = _offset;

		if(std::isnan(size.width) || std::isnan(size.height) || std::isnan(offset.x) || std::isnan(offset.y))
		{
			Log::error("guib::Align", "Child size/offset must be possible to calculate!");
			return;
		}
	}

	//--------------------------------//
	//---------- PreProcess ----------//
	//--------------------------------//
	void Widget::treePreProcess()
	{
		startPreProcess();
		{
			preProcessSizeOffset();
			// Local to global offset (local+parent)
			if(_parent) _offset += _parent->getOffset();
			preProcess();
		}
		endPreProcess();
	}

	void Widget::startPreProcess()
	{
		if(_child)
			_child->setParent(this);
	}

	void Widget::endPreProcess()
	{
		// Finish this widget pre process and call child pre process
		if(_child)
			_child->treePreProcess();
	}

	void Widget::preProcessSizeOffset()
	{
		// Convert from UNIT_PIXEL to UNIT_SCREEN
		solveDimensionsPixel();
		// Convert from UNIT_PERCENT to UNIT_SCREEN
		solveDimensionsPercent();
	}

	void Widget::preProcess()
	{
		// Executes one time after while the tree is being created
		// NOTE: child not preprocessed yet
	}

	//--------------------------------//
	//---------- Tree Wrap -----------//
	//--------------------------------//
	void Widget::wrapChild()
	{
		// Sometimes the child of this widget needs to now the parent of this widget size/offset to calculate your own size/offset
		//--------
		// parent
		//   |
		//  this
		//   |
		// child
		fillParent();

		// Wrap child if it exists
		if(_child)
		{
			Log::debug("Widget", "parent size:$0 offset:$1", _size.toString(), _offset.toString());
			Size childSize;
			Offset childOffset;
			_child->parentAsksSizeOffset(childSize, childOffset);

			_size = childSize;
			_offset = childOffset;
			Log::debug("Widget", "child size:$0 offset:$1", _size.toString(), _offset.toString());
		}
	}

	void Widget::fillParent()
	{
		Size parentSize;
		Offset parentOffset;

		getParentSizeOffset(parentSize, parentOffset);
		// Calculate local coord size and offset
		_size = parentSize;
		_offset = {0,0};
	}

	//--------------------------------//
	//------------ Update ------------//
	//--------------------------------//
	void Widget::update()
	{
		//startPreProcess();
		//preProcessSizeOffset();
		//if(_parent) _offset += _parent->getOffset();

		//Log::debug("Widget", "Update [w]$0[] with [w]$1 []--[w] $2", _type, _size.toString(), _offset.toString());

		if(_child)
			_child->update();
	}

	void Widget::addOffsetTree(Offset offset)
	{
		_offset += offset;
		//Log::debug("Widget", "AddOff [y]$0[] with [w]$1", _type, _offset.toString());
		if(_child)
			_child->addOffsetTree(offset);
	}

	//--------------------------------//
	//----------- SolveDim -----------//
	//--------------------------------//
	void Widget::solveDimensionsPercent()
	{
		// Convert from UNIT_PERCENT to UNIT_SCREEN
		Size parentSize;
		Offset parentOffset;
		getParentSizeOffset(parentSize, parentOffset);

		if(_size.unitW == guib::UNIT_PERCENT)
		{
			_size.width *= parentSize.width;
			_size.unitW = guib::UNIT_SCREEN;
		}
		if(_size.unitH == guib::UNIT_PERCENT)
		{
			_size.height *= parentSize.height;
			_size.unitH = guib::UNIT_SCREEN;
		}

		if(_offset.unitX == guib::UNIT_PERCENT)
		{
			_offset.x = parentSize.width*_offset.x;
			_offset.unitX = guib::UNIT_SCREEN;
		}
		if(_offset.unitY == guib::UNIT_PERCENT)
		{
			_offset.y = parentSize.height*_offset.y;
			_offset.unitY = guib::UNIT_SCREEN;
		}
	}

	void Widget::solveDimensionsPixel()
	{
		// Convert from UNIT_PIXEL to UNIT_SCREEN
		float invWidth = 1.0f/state::screenSize.width;
		float invHeight = 1.0f/state::screenSize.height;

		if(_size.unitW == guib::UNIT_PIXEL)
		{
			_size.width *= invWidth;
			_size.unitW = guib::UNIT_SCREEN;
		}
		if(_size.unitH == guib::UNIT_PIXEL)
		{
			_size.height *= invHeight;
			_size.unitH = guib::UNIT_SCREEN;
		}

		if(_offset.unitX == guib::UNIT_PIXEL)
		{
			_offset.x *= invWidth;
			_offset.unitX = guib::UNIT_SCREEN;
		}
		if(_offset.unitY == guib::UNIT_PIXEL)
		{
			_offset.y *= invHeight;
			_offset.unitY = guib::UNIT_SCREEN;
		}
	}
}

