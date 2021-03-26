//--------------------------------------------------
// GuiB
// window.cpp
// Date: 2020-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/window.h>
#include <atta/graphics/gui/widgets/widgets.h>
#include <atta/helpers/log.h>

namespace guib
{
	Window::Window(WindowInfo info):
		Widget({info.offset, info.size}),
		_name(info.name), _color(info.color),
		_closable(info.closable), _fixedSize(info.fixedSize),
		_minimizable(info.minimizable), _movable(info.movable),
		_root(nullptr), _windowChild(info.child),
		_minimized(false), _closed(false)
	{
		Widget::setType("Window");
		updateTree();
	}

	void Window::updateTree()
	{
		Widget** c = Widget::getChildPtr();
		if(*c!=nullptr)
		{
			delete *c;
			*c=nullptr;
		}

		Widget* minimizeButton = nullptr;
		if(_minimizable)
		{
			minimizeButton = new guib::ClickDetector(
							{
								.onClick = [&](){
									_minimized = !_minimized;
									_windowChildVisibility->setVisible(!_minimized);
								},
								.child = new guib::Box(
								{
									.color = {.8,.8,.3,1},
									.radius = {1,1,1,1},
									.size  = {10,10, guib::UNIT_PIXEL, guib::UNIT_PIXEL}
								}),
							});
		}
		else
			minimizeButton = new guib::Box({.color={0,0,0,0}, .size={0,0}});

		Widget* closeButton = nullptr;
		if(_closable)
		{
			closeButton = new guib::ClickDetector(
							{
								.onClick = [&](){
									_closed = true;
									((Visibility*)_root)->setVisible(false);
								},
								.child = new guib::Box(
								{
									.color = {.8,.3,.3,1},
									.radius = {1,1,1,1},
									.size  = {10,10, guib::UNIT_PIXEL, guib::UNIT_PIXEL}
								}),
							});
		}
		else
			closeButton = new guib::Box({.color={0,0,0,0}, .size={0,0}});

		Widget* topBar = new guib::Box(
						{
							.color = {.15,.15,.15,1},
							.size  = {1,20, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
							.child = new guib::Row(
							{
								.hAlignment = guib::ALIGN_START,
								.vAlignment = guib::ALIGN_CENTER,
								.children = {
									//new guib::Box(
									//{
									//	.color = {0,0,0,0},
									//	.size  = {4,1, guib::UNIT_PIXEL}
									//}),
									new guib::Box(
									{
										.color = {1,0,1,0},
										.size  = {.5,1},
										.child = new guib::Text(
										{
											.color = {0.8, 0.8, 0.8, 1.0},
											.text = _name,
											.textSize = 14
										})
									}),
									new guib::Box(
									{
										.color = {0,0,0,0},
										.size  = {.5,1},
										.child = new guib::Row(
										{
											.hAlignment = guib::ALIGN_END,
											.vAlignment = guib::ALIGN_CENTER,
											.children = {
												minimizeButton,
												new guib::Box(
												{
													.color = {0,0,0,0},
													.size  = {4,1, guib::UNIT_PIXEL}
												}),
												closeButton,
												new guib::Box(
												{
													.color = {0,0,0,0},
													.size  = {4,1, guib::UNIT_PIXEL}
												})
											}
										})
									})
								}
							})
						});

		_root = new guib::Visibility(
			{
				.visible = !_closed,
				.child = new guib::Draggable(
				{
					.active = _movable,
					.widgetToHover = topBar,
					.widgetToDrag = this,
					.child = new guib::Box(
					{
						.color = {.2,.2,.2,0},
						.size  = {1, 1, guib::UNIT_PERCENT, guib::UNIT_PERCENT},
						.child = new guib::Column(
						{
							.hAlignment = guib::ALIGN_CENTER,
							.vAlignment = guib::ALIGN_START,
							.children = {
								topBar,
								_windowChildVisibility=new Visibility(
								{
									.visible = !_minimized,
									.child= new Box(
									{
										.color = _color,
										.child = new Protect(
										{
											.child=_windowChild
										})
									})
								})
							}
						})
					})
				})
			}); 
		Widget::setChild(_root);
	}
}

