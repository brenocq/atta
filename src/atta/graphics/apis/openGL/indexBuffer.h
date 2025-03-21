//--------------------------------------------------
// Atta Graphics Module
// indexBuffer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_INDEX_BUFFER_H
#define ATTA_GRAPHICS_APIS_OPENGL_INDEX_BUFFER_H

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

#endif // ATTA_GRAPHICS_APIS_OPENGL_INDEX_BUFFER_H
