//--------------------------------------------------
// Atta Graphics Module
// graphicsAPI.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/graphicsAPI.h>

namespace atta::graphics {

GraphicsAPI::GraphicsAPI(Type type, std::shared_ptr<Window> window) : _type(type), _window(window) {}

} // namespace atta::graphics
