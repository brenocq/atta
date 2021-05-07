//--------------------------------------------------
// GuiB
// topBar.cpp
// Date: 2021-01-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/topBar.h>
#include <atta/graphics/gui/widgets/box.h>
#include <atta/graphics/gui/widgets/column.h>
#include <atta/graphics/gui/widgets/row.h>
#include <atta/graphics/gui/widgets/clickDetector.h>
#include <atta/graphics/gui/guiState.h>
#include <atta/helpers/log.h>

namespace guib
{
	TopBar::TopBar(TopBarInfo info):
		Widget({
				.offset = {0,0}, 
				.size={1, 26, guib::UNIT_PERCENT, guib::UNIT_PIXEL}}),
		_color(info.color)
	{
		Widget::setType("TopBar");

		Widget* minimizeButton = new guib::ClickDetector(
						{
							.onClick = [&](){
							},
							.child = new guib::Box(
							{
								.color = {.8,.8,.3,1},
								.radius = {1,1,1,1},
								.size  = {10,10, guib::UNIT_PIXEL, guib::UNIT_PIXEL}
							}),
						});

		Widget* closeButton = new guib::ClickDetector(
							{
								.onClick = [&](){
									state::shouldClose = true;
								},
								.child = new guib::Box(
								{
									.color = {.8,.3,.3,1},
									.radius = {1,1,1,1},
									.size  = {10,10, guib::UNIT_PIXEL, guib::UNIT_PIXEL}
								}),
							});

		Widget::setChild(
			new guib::Column(
			{
				.children = {
					new guib::Box(
					{
						.color = info.color,
						.size  = {1, 3, UNIT_PERCENT, UNIT_PIXEL},
					}),
					new guib::Box(
					{
						.color = info.color,
						.size  = {1, 20, UNIT_PERCENT, UNIT_PIXEL},
						.child = new Row(
						{
							.children =	{
								new guib::Box(
								{
									.color = info.color,
									.size  = {.8, 1},
									.child = new Row(
									{
										.children = info.buttons
									})
								}),
								new guib::Box(
								{
									.color = info.color,
									.size  = {.2, 1},
									.child = new Row(
									{
										.hAlignment = ALIGN_END,
										.vAlignment = ALIGN_CENTER,
										.children = {
											minimizeButton, 
											new guib::Box((BoxInfo){.color = info.color, .size = {5, 1, UNIT_PIXEL}}),
											closeButton,
											new guib::Box((BoxInfo){.color = info.color, .size = {5, 1, UNIT_PIXEL}}),
										}
									})
								})
							}
						})
					}),
					new guib::Box(
					{
						.color = info.color,
						.size  = {1, 3, UNIT_PERCENT, UNIT_PIXEL},
					}),
				}
			})
		);
	}
}

