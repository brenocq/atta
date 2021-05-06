//--------------------------------------------------
// GuiB
// buttonText.h
// Date: 2021-05-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_BUTTON_TEXT_H
#define GUIB_BUTTON_TEXT_H

#include <atta/graphics/gui/widgets/widget.h>
#include <atta/graphics/gui/widgets/box.h>
#include <vector>
#include <functional>

namespace guib {
	struct ButtonTextInfo {
		std::string text = "Button";
		Color color = {.4,.4,.4,1};
		Color hoverColor = {.45,.45,.45,1};
		Color clickColor = {.5,.5,.5,1};
		std::function<void()> onClick;
		BoxRadius radius = {.5,.5,.5,.5};
	};

	class ButtonText : public Widget
	{
		public:
			ButtonText(ButtonTextInfo info);

		private:
			Size calculateButtonSize(std::string text);
			unsigned _paddingH;
			unsigned _paddingV;
	};
}

#endif// GUIB_BUTTON_TEXT_H
