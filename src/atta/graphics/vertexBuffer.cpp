// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/vertexBuffer.h>

namespace atta::graphics {

VertexBuffer::VertexBuffer(const CreateInfo& info) : _layout(info.layout), _usage(info.usage), _size(info.size), _data(info.data) {}

uint32_t VertexBuffer::getSize() const { return _size; };
BufferLayout VertexBuffer::getLayout() const { return _layout; }
uint32_t VertexBuffer::getCount() const { return _size / _layout.getStride(); }

} // namespace atta::graphics
