//--------------------------------------------------
// Atta Graphics Module
// vertexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vertexBuffer.h>

namespace atta::graphics {

VertexBuffer::VertexBuffer(const CreateInfo& info) : _layout(info.layout), _usage(info.usage), _size(info.size), _data(info.data) {}

} // namespace atta::graphics
