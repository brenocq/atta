//--------------------------------------------------
// Atta Graphics Module
// indexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/indexBuffer.h>

namespace atta::graphics {
IndexBuffer::IndexBuffer(const CreateInfo& info) : _offset(info.offset), _size(info.size), _data(info.data) {}
} // namespace atta::graphics
