//--------------------------------------------------
// GuiB
// button.cpp
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "button.h"
#include "widgets.h"

namespace guib
{
	Button::Button(ButtonInfo info):
		Widget({.size = info.size}),
		_color(info.color), _hoverColor(info.hoverColor), _clickColor(info.clickColor),
		_onClick(info.onClick), _radius(info.radius), 
		_hovering(false), _clicking(false)
	{
		Widget::setType("Button");

		_boxChild = new guib::Box(
				{
					.color = _color,
					.radius = _radius,
					.size  = {1,1},
					.child = info.child
				});

		Widget::setChild(new guib::ClickDetector(
			{
				.onClick = [&](){
					if(_onClick)
						_onClick();
					_clicking = true;
					_boxChild->setColor(_clickColor);
				},
				.onStopClick = [&](){
					_clicking = false;
					if(!_hovering)
						_boxChild->setColor(_color);
					else
						_boxChild->setColor(_hoverColor);
				},
				.onHover = [&](){
					_hovering = true;
					if(!_clicking)
						_boxChild->setColor(_hoverColor);
				},
				.onStopHover = [&](){
					_hovering = false;
					if(!_clicking)
						_boxChild->setColor(_color);
				},
				.child = _boxChild
			})
		);
	}

	Button::~Button()
	{
	}
}

