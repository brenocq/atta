//--------------------------------------------------
// GuiB
// box.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_BOX_H
#define GUIB_BOX_H

#include "widget.h"

namespace guib {
	struct BoxRadius {
		float topLeft = 0.0f;
		float bottomLeft = 0.0f;
		float bottomRight = 0.0f;
		float topRight = 0.0f;
		Unit unitTL = UNIT_PERCENT;
		Unit unitBL = UNIT_PERCENT;
		Unit unitBR = UNIT_PERCENT;
		Unit unitTR = UNIT_PERCENT;

		std::string toString()
		{
			return std::string("BoxRadius {topLeft=")+std::to_string(topLeft)+", bottomLeft="+std::to_string(bottomLeft)+", bottomRight="+std::to_string(bottomRight)+", topRight="+std::to_string(topRight)+"}";
		}
	};

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
