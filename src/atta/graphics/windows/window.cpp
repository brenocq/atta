//--------------------------------------------------
// Atta Graphics Module
// window.cpp
// Date: 2021-10-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/windows/window.h>

namespace atta::graphics {

Window::Window(const CreateInfo& info) : _title(info.title), _width(info.width), _height(info.height) {}

} // namespace atta::graphics
