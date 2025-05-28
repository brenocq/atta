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
    file::write<uint32_t>(os, getWidth());
    file::write<uint32_t>(os, getHeight());
}

void Renderer::deserialize(std::istream& is) {
    // Name should be file::read before to know which renderer to deserialize
    uint32_t width, height;
    file::read<uint32_t>(is, width);
    file::read<uint32_t>(is, height);
    resize(width, height);
}

} // namespace atta::graphics
