//--------------------------------------------------
// GuiB
// box.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_BOX_H
#define GUIB_BOX_H

#include "widget.h"
#include "widgetStructs.h"

namespace guib {
	struct BoxInfo {
		Color color = {.3,.3,.3,1};
		BoxRadius radius = {.0,.0,.0,.0};
		Size size = {1,1};
		Widget* child = nullptr;
	};

	class Box : public Widget
	{
		public:
			Box(BoxInfo boxInfo);
			~Box();

			void render();

			//---------- Getters and Setters ----------//
			void setColor(Color color) { _color=color; }
			Color getColor() const { return _color; }
			BoxRadius getRadius() const { return _radius; }

		private:
			Color _color;
			BoxRadius _radius;
	};
}

#endif// GUIB_BOX_H
