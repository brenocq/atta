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
		ALIGN_RIGHT=0,
		ALIGN_LEFT,
		ALIGN_CENTER
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

		Size& operator/=(const Size& s)
		{
			this->width /= s.width;
			this->height /= s.height;
			return *this;
		}

		std::string toString()
		{
			return std::string("Size {width=")+std::to_string(width)+", height="+std::to_string(height)+"}";
		}
	};

	class ClickDetector;
	struct ClickDetectorArea {
		Offset offset;
		Size size;
		ClickDetector* clickDetector = nullptr;
	};

}
