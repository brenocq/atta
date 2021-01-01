//--------------------------------------------------
// GuiB
// text.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "text.h"

namespace guib
{
	Text::Text(TextInfo info):
		Widget({.offset={0,0}, .size={0, info.textSize, UNIT_PERCENT, UNIT_PIXEL}}),
		_color(info.color), _text(info.text), _textSize(info.textSize)
	{
		Widget::setType("Text");
	}

	Text::~Text()
	{

	}

	void Text::render()
	{

	}
}
