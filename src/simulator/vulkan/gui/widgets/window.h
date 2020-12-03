//--------------------------------------------------
// GuiB
// window.h
// Date: 2020-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_WINDOW_H
#define GUIB_WINDOW_H

#include "widget.h"
#include <vector>

namespace guib {
	struct WindowInfo {
		std::string name = "Unnamed";
		bool closable = true;
		bool fixedSize = true;
		bool minimizable = true;
		bool movable = true;
		Offset offset = {100, 100, UNIT_PIXEL, UNIT_PIXEL};
		Size size = {300, 200, UNIT_PIXEL, UNIT_PIXEL};
		Widget* child = nullptr;
	};

	class Window : public Widget
	{
		public:
			Window(WindowInfo info);
			~Window();

			//---------- Getters and Setters ----------//
			bool getClosable() const { return _closable; }
			bool getMinimizable() const { return _minimizable; }
			bool getMovable() const { return _movable; }
			bool getFixedSize() const { return _fixedSize; }

		private:
			std::string _name;
			bool _closable;
			bool _fixedSize;
			bool _minimizable;
			bool _movable;

			Widget* _root;
	};
}

#endif// GUIB_WINDOW_H
