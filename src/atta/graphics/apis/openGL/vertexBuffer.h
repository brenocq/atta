// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/vertexBuffer.h>

namespace atta::graphics::gl {

class VertexBuffer final : public gfx::VertexBuffer {
  public:
    VertexBuffer(const VertexBuffer::CreateInfo& info);
    ~VertexBuffer();

    void bind() const override;
    void update(const uint8_t* data, uint32_t size) override;

    OpenGLId getHandle() const { return _id; }

  private:
    static GLenum convertUsage(Usage usage);
    static GLenum convertBaseType(BufferLayout::Element::Type type);

    OpenGLId _id;
};

} // namespace atta::graphics::gl
