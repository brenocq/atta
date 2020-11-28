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
	struct BoxInfo {
		Color color = {.3,.3,.3,1};
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

		private:
			Color _color;
	};
}

#endif// GUIB_BOX_H
