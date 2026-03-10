// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/apis/graphicsAPI.h>

namespace atta::graphics {

GraphicsAPI::GraphicsAPI(Type type, std::shared_ptr<Window> window) : _type(type), _window(window) {}

} // namespace atta::graphics
