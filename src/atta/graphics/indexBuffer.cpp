// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/indexBuffer.h>

namespace atta::graphics {

IndexBuffer::IndexBuffer(const CreateInfo& info) : _offset(info.offset), _size(info.size), _data(info.data) {}

} // namespace atta::graphics
