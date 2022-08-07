//--------------------------------------------------
// Atta Graphics Module
// openGLIndexBuffer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_INDEX_BUFFER_H
#define ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_INDEX_BUFFER_H
#include <atta/graphics/indexBuffer.h>
#include <atta/graphics/rendererAPIs/openGL/base.h>

namespace atta::graphics {
class OpenGLIndexBuffer final : public IndexBuffer {
  public:
    OpenGLIndexBuffer(const IndexBuffer::CreateInfo& info);
    ~OpenGLIndexBuffer();

    void bind() const override;

    OpenGLId getId() const { return _id; }

  private:
    OpenGLId _id;
};
} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_INDEX_BUFFER_H
