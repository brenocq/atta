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
    file::write(os, _name);
    file::write(os, _width);
    file::write(os, _height);
}

void Renderer::deserialize(std::istream& is) {
    // Name should be file::read before to know which renderer to deserialize
    decltype(_width) width, height;
    file::read(is, width);
    file::read(is, height);
    resize(width, height);
}

} // namespace atta::graphics
