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
		Widget* widgetToHover = nullptr;// nullptr -> this
		Widget* widgetToDrag = nullptr;// nullptr -> this
		Widget* child = nullptr;
	};

	class Draggable : public Widget
	{
		public:
			Draggable(DraggableInfo info);

			void preProcessSizeOffset() override;
			void preProcess() override;

			//---------- Getters and Setters ----------//
			bool getActive() const { return _active; }
			Widget* getWidgetToHover() const { return _widgetToHover; }
			Widget* getWidgetToDrag() const { return _widgetToDrag; }

		private:
			bool _active;
			Widget* _widgetToHover;
			Widget* _widgetToDrag;

			int _draggableStateIndex;
	};
}

#endif// GUIB_DRAGGABLE_H
