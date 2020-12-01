//--------------------------------------------------
// GuiB
// clickDetector.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_CLICK_DETECTOR_H
#define GUIB_CLICK_DETECTOR_H

#include "widget.h"
#include <functional>

namespace guib {
	struct ClickDetectorInfo {
		std::function<void()> onClick;
		Widget* child = nullptr;
	};

	class ClickDetector : public Widget
	{
		public:
			ClickDetector(ClickDetectorInfo info);
			~ClickDetector();

			//---------- Getters and Setters ----------//
			std::function<void()> getOnClick() const { return _onClick; }

		private:
			std::function<void()> _onClick;
	};
}

#endif// GUIB_CLICK_DETECTOR_H
