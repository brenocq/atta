// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/windows/window.h>

namespace atta::graphics {

Window::Window(const CreateInfo& info) : _title(info.title), _width(info.width), _height(info.height) {}

} // namespace atta::graphics
