//--------------------------------------------------
// GuiB
// widgetStructs.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace guib
{
	struct Color {
		float r;
		float g;
		float b;
		float a;
	};

	struct Offset {
		float x;
		float y;
	};

	struct Size {
		float width;
		float height;

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
	};

}
