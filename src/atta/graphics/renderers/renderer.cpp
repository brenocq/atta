//--------------------------------------------------
// Atta Graphics Module
// renderer.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/serializer.h>
#include <atta/graphics/renderers/renderer.h>

namespace atta::graphics {
void Renderer::serialize(std::ostream& os) {
    write(os, _name);
    write(os, _width);
    write(os, _height);
}

void Renderer::deserialize(std::istream& is) {
    // Name should be read before to know which renderer to deserialize
    decltype(_width) width, height;
    read(is, width);
    read(is, height);
    resize(width, height);
}
} // namespace atta::graphics
