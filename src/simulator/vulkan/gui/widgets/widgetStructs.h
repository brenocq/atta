//--------------------------------------------------
// GuiB
// widgetStructs.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <string>

namespace guib
{
	struct Color {
		float r;
		float g;
		float b;
		float a;
	};

	enum Alignment {
		ALIGN_START=0,
		ALIGN_CENTER,
		ALIGN_END
	};

	enum Unit {
		UNIT_PIXEL=0,
		UNIT_PERCENT
	};

	struct Offset {
		float x;
		float y;
		Unit unitX = UNIT_PERCENT;
		Unit unitY = UNIT_PERCENT;
		
		Offset& operator+=(const Offset& o)
		{
			this->x += o.x;
			this->y += o.y;
			return *this;
		}

		Offset& operator-=(const Offset& o)
		{
			this->x -= o.x;
			this->y -= o.y;
			return *this;
		}

		Offset operator+(const Offset& o) const
		{
			return {x+o.x, y+o.y, unitX, unitY};
		}

		Offset operator-(const Offset& o) const
		{
			return {x-o.x, y-o.y, unitX, unitY};
		}

		std::string toString()
		{
			return std::string("Offset {x=")+std::to_string(x)+", y="+std::to_string(x)+"}";
		}
	};

	struct Size {
		float width;
		float height;
		Unit unitW = UNIT_PERCENT;
		Unit unitH = UNIT_PERCENT;

		Size& operator+=(const Size& s)
		{
			this->width += s.width;
			this->height += s.height;
			return *this;
		}

		Size& operator*=(const Size& s)
		{
			this->width *= s.width;
			this->height *= s.height;
			return *this;
		}

		Size operator*(const Size& s) const
		{
			return {width+s.width, height+s.height, unitW, unitH};
		}

		Size& operator/=(const Size& s)
		{
			this->width /= s.width;
			this->height /= s.height;
			return *this;
		}

		std::string toString()
		{
			return std::string("Size {width=")+std::to_string(width)+", height="+std::to_string(height)+", unitW="+
				std::string(unitW==UNIT_PERCENT? "UNIT_PERCENT":"UNIT_PIXEL")+", unitH="+
				std::string(unitH==UNIT_PERCENT? "UNIT_PERCENT":"UNIT_PIXEL")+
				"}";
		}
	};

	class ClickDetector;
	struct ClickDetectorArea {
		Offset offset;
		Size size;
		ClickDetector* clickDetector = nullptr;
	};

	class Draggable;
	struct DragDetectorArea {
		Offset offset;
		Size size;
		Draggable* draggable = nullptr;
	};
}
