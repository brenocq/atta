//--------------------------------------------------
// GuiB
// widget.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "widget.h"
#include "../guiState.h"
#include "simulator/helpers/log.h"

namespace guib
{
	Widget::Widget(WidgetInfo widgetInfo):
		_size(widgetInfo.size), _offset(widgetInfo.offset), _child(widgetInfo.child), _parent(nullptr), _type("Widget"),
		_widgetInfo(widgetInfo)
	{

		// Convert from UNIT_PIXEL to UNIT_SCREEN
		solveDimensionsPixel();
		// Conversion from UNIT_PERCENT to UNIT_SCREEN occurs in preProcess or called by parent
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
		//Log::debug("Widget", "Render [c]$0", _type);
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

	void Widget::getSizeOffset(Size &size, Offset &offset)
	{
		// The parent can call this method if it needs to modify the child size/offset
		// Ex: Align
		solveDimensionsPercent();
		size = _size;	
		offset = _offset;	
	}

	//--------------------------------//
	//---------- PreProcess ----------//
	//--------------------------------//
	void Widget::startPreProcess()
	{
		//Log::debug("Widget", "Start preprocess [c]$0", _type);
		// Convert from UNIT_PERCENT to UNIT_SCREEN
		// It is made in pre preocess because it needs to have a reference to the parent, which occurs after the tree is created
		solveDimensionsPercent();
		if(_child)
			_child->setParent(this);
	}

	void Widget::endPreProcess()
	{
		if(_child)
			_child->preProcess();
	}

	void Widget::preProcess()
	{
		startPreProcess();
		endPreProcess();
	}

	//--------------------------------//
	//------------ Update ------------//
	//--------------------------------//
	void Widget::update()
	{
		if(_child)
			_child->update();
	}

	//--------------------------------//
	//----------- SolveDim -----------//
	//--------------------------------//
	void Widget::solveDimensionsPercent()
	{
		// Convert from UNIT_PERCENT to UNIT_SCREEN
		if(_parent == nullptr)
			return;

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
			_offset.x = parentOffset.x + parentSize.width*_offset.x;
			_offset.unitX = guib::UNIT_SCREEN;
		}
		if(_offset.unitY == guib::UNIT_PERCENT)
		{
			_offset.y = parentOffset.y + parentSize.height*_offset.y;
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

