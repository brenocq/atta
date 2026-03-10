// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/indexBuffer.h>

namespace atta::graphics::gl {

class IndexBuffer final : public gfx::IndexBuffer {
  public:
    IndexBuffer(const IndexBuffer::CreateInfo& info);
    ~IndexBuffer();

    void bind() const override;

    OpenGLId getHandle() const { return _id; }

  private:
    OpenGLId _id;
};

} // namespace atta::graphics::gl
