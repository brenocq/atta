//--------------------------------------------------
// Atta UI Module
// align.h
// Date: 2024-01-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WIDGETS_ALIGN_H
#define ATTA_UI_WIDGETS_ALIGN_H

namespace atta::ui {

void verticalAlign(float height, std::function<void()> func);

void textRightAligned(const std::string& text);

} // namespace atta::ui

#endif // ATTA_UI_WIDGETS_ALIGN_H
