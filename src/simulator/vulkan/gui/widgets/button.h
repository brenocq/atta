//--------------------------------------------------
// GuiB
// button.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_BUTTON_H
#define GUIB_BUTTON_H

#include "widget.h"
#include "box.h"
#include <vector>
#include <functional>

namespace guib {
	struct ButtonInfo {
		Color color = {.4,.4,.4,1};
		Color hoverColor = {.45,.45,.45,1};
		Color clickColor = {.5,.5,.5,1};
		std::function<void()> onClick;
		Size size = {.8,.8};
		BoxRadius radius = {.0,.0,.0,.0};
		Widget* child = nullptr;
	};

	class Button : public Widget
	{
		public:
			Button(ButtonInfo info);
			~Button();

			//---------- Getters and Setters ----------//
			Color getColor() const { return _color; }
			Color getHoverColor() const { return _hoverColor; }
			Color getClickColor() const { return _clickColor; }
			BoxRadius getRadius() const { return _radius; }

			bool getHovering() const { return _hovering; }
			bool getClicking() const { return _clicking; }

		private:

			// Button parameters
			Color _color;
			Color _hoverColor;
			Color _clickColor;
			std::function<void()> _onClick;
			BoxRadius _radius;

			Box* _boxChild;

			// Button state
			bool _hovering;
			bool _clicking;
	};
}

#endif// GUIB_BUTTON_H
