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
		Color color = {.3,.3,.3,1};
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

			void render();

			//---------- Getters and Setters ----------//
			bool getClosable() const { return _closable; }
			bool getMinimizable() const { return _minimizable; }
			bool getMovable() const { return _movable; }
			bool getFixedSize() const { return _fixedSize; }

			bool getClosed() const { return _closed; }
			bool getMinimized() const { return _minimized; }

		private:
			void updateTree();

			// Window parameters
			std::string _name;
			Color _color;
			bool _closable;
			bool _fixedSize;
			bool _minimizable;
			bool _movable;

			Widget* _root;
			Widget* _windowChild;

			// Window state
			bool _minimized;
			bool _closed;
	};
}

#endif// GUIB_WINDOW_H
