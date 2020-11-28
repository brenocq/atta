//--------------------------------------------------
// GuiB
// padding.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_PADDING_H
#define GUIB_PADDING_H

#include "widget.h"
#include <vector>

namespace guib {
	struct PaddingValues  {
		float top = 0.1f;
		float right = 0.1f;
		float bottom = 0.1;
		float left = 0.1;

		static PaddingValues symmetric(float horizontal, float vertical)
		{
			return {vertical, horizontal, vertical, horizontal};
		}

		static PaddingValues all(float value)
		{
			return {value, value, value, value};
		}
	};

	struct PaddingInfo {
		PaddingValues padding = {};
		Widget* child = nullptr;
	};

	class Padding : public Widget
	{
		public:
			Padding(PaddingInfo paddingInfo);
			~Padding();

			//---------- Getters and Setters ----------//
			PaddingValues getPadding() const { return _padding; }

		private:
			PaddingValues _padding;
	};
}

#endif// GUIB_PADDING_H
