//--------------------------------------------------
// Atta Graphics Module
// mesh.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/mesh.h>
#include <atta/graphics/interface.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

namespace atta::graphics::gl {

Mesh::Mesh(CreateInfo info) : gfx::Mesh(info), _id(0) {
    glGenVertexArrays(1, &_id);
    glBindVertexArray(_id);
    _vertexBuffer = gfx::create<gfx::VertexBuffer>(info.vertexBufferInfo);
    _indexBuffer = gfx::create<gfx::IndexBuffer>(info.indexBufferInfo);
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (_id > 0)
        glDeleteVertexArrays(1, &_id);
}

void Mesh::draw() {
    glBindVertexArray(_id);
    glDrawElements(GL_TRIANGLES, _indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace atta::graphics::gl
