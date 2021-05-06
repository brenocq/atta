//--------------------------------------------------
// GuiB
// buttonImage.h
// Date: 2021-05-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_BUTTON_IMAGE_H
#define GUIB_BUTTON_IMAGE_H

#include <atta/graphics/gui/widgets/widget.h>
#include <functional>

namespace guib {
	struct ButtonImageInfo {
		std::string image = "";
		Color imageColor = {1,1,1,1};
		Color color = {.4,.4,.4,1};
		Color hoverColor = {.45,.45,.45,1};
		Color clickColor = {.5,.5,.5,1};
		std::function<void()> onClick;
		BoxRadius radius = {.5,.5,.5,.5};
	};

	class ButtonImage : public Widget
	{
		public:
			ButtonImage(ButtonImageInfo info);

		private:
	};
}

#endif// GUIB_BUTTON_IMAGE_H
