//--------------------------------------------------
// GuiB
// widget.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_WIDGET_H
#define GUIB_WIDGET_H
#include <string>

#include "widgetStructs.h"

namespace guib {
	class Widget;

	struct WidgetInfo {
		Offset offset = {0,0};
		Size size = {1,1};
		Widget* child = nullptr;
	};


	class Widget
	{
		public:
			Widget(WidgetInfo widgetInfo);
			~Widget();

			//---------- Getters and Setters ----------//
			void setChild(Widget* child) { _child=child; }
			Widget* getChild() const { return _child; }
			std::string getType() const { return _type; }
			void setSize(Size size) { _size=size; }
			Size getSize() const { return _size; }

			static Size screenSize;
		protected:
			void setType(std::string type) { _type=type; }
			Size _size;
			Offset _offset;

		private:
			Widget* _child;
			std::string _type;
	};
}

#endif// GUIB_WIDGET_H
