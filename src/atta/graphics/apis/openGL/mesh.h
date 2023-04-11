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
#include <atta/graphics/mesh.h>

namespace atta::graphics::gl {

class Mesh final : public gfx::Mesh {
  public:
    Mesh(StringId sid);
    ~Mesh();

    void draw() override;

  private:
    OpenGLId _id;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_MESH_H
