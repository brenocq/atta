// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_GRAPHICS_APIS_OPENGL_MESH_H
#define ATTA_GRAPHICS_APIS_OPENGL_MESH_H

#include <atta/graphics/apis/openGL/indexBuffer.h>
#include <atta/graphics/apis/openGL/vertexBuffer.h>
#include <atta/graphics/mesh.h>
#include <atta/graphics/pipeline.h>

namespace atta::graphics::gl {

class Mesh final : public gfx::Mesh {
  public:
    Mesh(CreateInfo info);
    ~Mesh();

    void draw(Pipeline::Primitive primitive, size_t numVertices = 0);

    OpenGLId getHandle() const { return _id; }

    static GLenum convert(Pipeline::Primitive primitive);

  private:
    OpenGLId _id;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_MESH_H
