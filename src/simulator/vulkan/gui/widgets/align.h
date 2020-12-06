//--------------------------------------------------
// GuiB
// align.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_ALIGN_H
#define GUIB_ALIGN_H

#include "widget.h"
#include <vector>

namespace guib {
	struct AlignInfo {
		Alignment hAlignment = ALIGN_START;
		Alignment vAlignment = ALIGN_START;
		Widget* child;
	};

	class Align : public Widget
	{
		public:
			Align(AlignInfo info);
			~Align();

			//---------- Getters and Setters ----------//
			Alignment getHAlignment() const { return _hAlignment; }
			Alignment getVAlignment() const { return _vAlignment; }

		private:
			Alignment _hAlignment;
			Alignment _vAlignment;
	};
}

#endif// GUIB_ALIGN_H
