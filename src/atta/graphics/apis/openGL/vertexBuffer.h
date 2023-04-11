//--------------------------------------------------
// Atta Graphics Module
// vertexBuffer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_VERTEX_BUFFER_H
#define ATTA_GRAPHICS_APIS_OPENGL_VERTEX_BUFFER_H

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/vertexBuffer.h>

namespace atta::graphics::gl {

class VertexBuffer final : public gfx::VertexBuffer {
  public:
    VertexBuffer(const VertexBuffer::CreateInfo& info);
    ~VertexBuffer();

    void bind() const override;

    OpenGLId getHandle() const { return _id; }

  private:
    static GLenum convertUsage(Usage usage);
    static GLenum convertBaseType(VertexBufferElement::Type type);

    OpenGLId _id;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_VERTEX_BUFFER_H
