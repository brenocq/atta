//--------------------------------------------------
// GuiB
// widget.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_WIDGET_H
#define GUIB_WIDGET_H
#include <string>

#include <atta/graphics/gui/widgets/widgetStructs.h>

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
			virtual ~Widget();

			virtual void render();
			void treePreProcess();
			virtual void update();
			virtual void addOffsetTree(Offset offset);

			//---------- Getters and Setters ----------//
			void setChild(Widget* child) { _child=child; }
			Widget* getChild() const { return _child; }
			void setParent(Widget* parent) { _parent=parent; }
			Widget* getParent() const { return _parent; }
			std::string getType() const { return _type; }
			void setSize(Size size) { _size=size; }
			Size getSize() const { return _size; }
			Size& getSize() { return _size; }
			void setOffset(Offset offset) { _offset=offset; }
			Offset getOffset() const { return _offset; }
			Offset& getOffset() { return _offset; }

			virtual void parentAsksSizeOffset(Size &size, Offset &offset);

		protected:
			void setType(std::string type) { _type=type; }
			Widget** getChildPtr() { return &_child; }
			void getParentSizeOffset(Size &pSize, Offset &pOffset);

			virtual void preProcessSizeOffset();
			virtual void preProcess();
			virtual void startPreProcess();
			virtual void endPreProcess();

			void solveDimensionsPercent();
			void solveDimensionsPixel();

			void wrapChild();
			void fillParent();

			Size _size;
			Offset _offset;
			Widget* _child;
			Widget* _parent;
			std::string _type;

		private:
			WidgetInfo _widgetInfo;
	};
}

#endif// GUIB_WIDGET_H
