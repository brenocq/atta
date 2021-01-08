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
		std::function<void()> onStopClick;
		std::function<void()> onRightClick;
		std::function<void()> onStopRightClick;
		std::function<void()> onHover;
		std::function<void()> onStopHover;
		Widget* child = nullptr;
	};

	class ClickDetector : public Widget
	{
		public:
			ClickDetector(ClickDetectorInfo info);

			void preProcessSizeOffset() override;
			void preProcess() override;
			void addOffsetTree(Offset offset) override;

			//---------- Getters and Setters ----------//
			std::function<void()> getOnClick() const { return _onClick; }
			std::function<void()> getOnStopClick() const { return _onStopClick; }
			std::function<void()> getOnRightClick() const { return _onRightClick; }
			std::function<void()> getOnStopRightClick() const { return _onStopRightClick; }
			std::function<void()> getOnHover() const { return _onHover; }
			std::function<void()> getOnStopHover() const { return _onStopHover; }
			bool getHovering() const { return _hovering; }
			bool getClicking() const { return _clicking; }
			bool getRightClicking() const { return _rightClicking; }
			void setHovering(bool hovering) { _hovering=hovering; }
			void setClicking(bool clicking) { _clicking=clicking; }
			void setRightClicking(bool rightClicking) { _rightClicking=rightClicking; }

		private:
			std::function<void()> _onClick;
			std::function<void()> _onStopClick;

			std::function<void()> _onRightClick;
			std::function<void()> _onStopRightClick;

			std::function<void()> _onHover;
			std::function<void()> _onStopHover;

			bool _hovering;
			bool _clicking;
			bool _rightClicking;

			int _clickableStateIndex;
	};
}

#endif// GUIB_CLICK_DETECTOR_H
