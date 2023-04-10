//--------------------------------------------------
// Atta Graphics Module
// mesh.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_MESH_H
#define ATTA_GRAPHICS_APIS_OPENGL_MESH_H

#include <atta/graphics/apis/openGL/indexBuffer.h>
#include <atta/graphics/apis/openGL/vertexBuffer.h>
#include <atta/utils/stringId.h>

namespace atta::graphics::gl {

class Mesh final {
  public:
    Mesh(StringId sid);
    ~Mesh();

    void draw();

  private:
    std::shared_ptr<VertexBuffer> _vertexBuffer;
    std::shared_ptr<IndexBuffer> _indexBuffer;
    StringId _sid;

    OpenGLId _id;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_MESH_H
