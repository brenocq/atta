//--------------------------------------------------
// GuiB
// widgetStructs.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_WIDGET_STRUCTS
#define GUIB_WIDGET_STRUCTS
#include <string>

namespace guib
{
	struct Color
	{
		float r=1;
		float g=1;
		float b=1;
		float a=1;
	};

	enum Alignment
	{
		ALIGN_START=0,
		ALIGN_CENTER,
		ALIGN_END,
		ALIGN_BETWEEN,
	};

	enum Unit
	{
		UNIT_PIXEL=0,
		UNIT_PERCENT,
		UNIT_SCREEN
	};

	struct Offset
	{
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

	struct Size
	{
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
			std::string strUW = "UNIT_PERCENT";
			std::string strUH = "UNIT_PERCENT";
			if(unitW == UNIT_PIXEL) strUW = "UNIT_PIXEL";
			else if(unitW == UNIT_SCREEN) strUW = "UNIT_SCREEN";
			if(unitH == UNIT_PIXEL) strUH = "UNIT_PIXEL";
			else if(unitH == UNIT_SCREEN) strUH = "UNIT_SCREEN";

			return std::string("Size {width=")+std::to_string(width)+", height="+std::to_string(height)+
				", unitW="+strUW+
				", unitH="+strUH+
				"}";
		}
	};

	struct BoxRadius
	{
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


	class ClickDetector;
	struct ClickDetectorArea
	{
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
#endif// GUIB_WIDGET_STRUCTS
