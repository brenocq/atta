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
		float bottom = 0.1f;
		float left = 0.1f;
		Unit unitT = UNIT_PERCENT;
		Unit unitR = UNIT_PERCENT;
		Unit unitB = UNIT_PERCENT;
		Unit unitL = UNIT_PERCENT;

		static PaddingValues symmetric(float horizontal, float vertical, 
				Unit unitH = UNIT_PERCENT, Unit unitV = UNIT_PERCENT)
		{
			return {vertical, horizontal, vertical, horizontal, unitV, unitH, unitV, unitH};
		}

		static PaddingValues all(float value, Unit unit = UNIT_PERCENT)
		{
			return {value, value, value, value, unit, unit, unit, unit};
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

			void startPreProcess() override;
			void preProcessSizeOffset() override;

			//---------- Getters and Setters ----------//
			PaddingValues getPadding() const { return _padding; }

		private:
			PaddingValues _padding;
	};
}

#endif// GUIB_PADDING_H
