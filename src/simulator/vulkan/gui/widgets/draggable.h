//--------------------------------------------------
// GuiB
// draggable.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_DRAGGABLE_H
#define GUIB_DRAGGABLE_H

#include "widget.h"
#include <functional>

namespace guib {
	struct DraggableInfo {
		bool active = true;
		Offset dragAreaOffset = {0,0};
		Size dragAreaSize = {1,1};
		Widget* widgetToDrag = nullptr;
		Widget* child = nullptr;
	};

	class Draggable : public Widget
	{
		public:
			Draggable(DraggableInfo info);
			~Draggable();

			void render();

			//---------- Getters and Setters ----------//
			bool getActive() const { return _active; }
			Offset getDragAreaOffset() const { return _dragAreaOffset; }
			Size getDragAreaSize() const { return _dragAreaSize; }
			Widget* getWidgetToDrag() const { return _widgetToDrag; }

		private:
			bool _active;
			Offset _dragAreaOffset;
			Size _dragAreaSize;
			Widget* _widgetToDrag;
	};
}

#endif// GUIB_DRAGGABLE_H
