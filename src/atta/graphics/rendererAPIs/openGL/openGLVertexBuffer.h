//--------------------------------------------------
// Atta Graphics Module
// openGLVertexBuffer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_VERTEX_BUFFER_H
#define ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_VERTEX_BUFFER_H
#include <atta/graphics/rendererAPIs/openGL/base.h>
#include <atta/graphics/vertexBuffer.h>

namespace atta::graphics {
class OpenGLVertexBuffer final : public VertexBuffer {
  public:
    OpenGLVertexBuffer(const VertexBuffer::CreateInfo& info);
    ~OpenGLVertexBuffer();

    void bind() const override;

    OpenGLId getId() const { return _id; }

  private:
    static GLenum convertUsage(Usage usage);
    static GLenum convertBaseType(VertexBufferElement::Type type);

    OpenGLId _id;
};
} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_VERTEX_BUFFER_H
