//--------------------------------------------------
// Atta Graphics Module
// openGLMesh.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_MESH_H
#define ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_MESH_H
#include <atta/core/stringId.h>
#include <atta/graphics/rendererAPIs/openGL/openGLIndexBuffer.h>
#include <atta/graphics/rendererAPIs/openGL/openGLVertexBuffer.h>

namespace atta::graphics {
class OpenGLMesh final {
  public:
    OpenGLMesh(StringId sid);
    ~OpenGLMesh();

    void draw();

  private:
    std::shared_ptr<OpenGLVertexBuffer> _vertexBuffer;
    std::shared_ptr<OpenGLIndexBuffer> _indexBuffer;
    StringId _sid;

    OpenGLId _id;
};
} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_MESH_H
