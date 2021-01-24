//--------------------------------------------------
// GuiB
// button.cpp
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
//#include "button.h"
//#include "widgets.h"
//
//namespace guib
//{
//	Button::Button(ButtonInfo info):
//		Widget({}),
//		_color(info.color), _hoverColor(info.hoverColor), _clickColor(info.clickColor),
//		_onClick(info.onClick), _radius(info.radius), 
//		_hovering(false), _clicking(false)
//	{
//		Widget::setType("Button");
//
//		_boxChild = new guib::Box(
//				{
//					.color = _color,
//					.radius = _radius,
//					.size  = {1,1},
//					.child = info.child
//				});
//
//		Widget::setChild(new guib::Button(
//			{
//				.color = info.color,
//				.hoverColor = {.35,.35,.35,1};
//				.clickColor = {.4,.4,.4,1};
//				. radius = {.0,.0,.0,.0};
//				. size = {.8,.8};
//				.onClick = [&](){
//				},
//				.onStopClick = [&](){
//				},
//				.onHover = [&](){
//				},
//				.onStopHover = [&](){
//				},
//			})
//		);
//	}
//}
//
