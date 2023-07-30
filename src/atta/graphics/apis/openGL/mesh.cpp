//--------------------------------------------------
// Atta Graphics Module
// mesh.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/mesh.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

namespace atta::graphics::gl {

Mesh::Mesh(CreateInfo info) : gfx::Mesh(info), _id(0) {
    glGenVertexArrays(1, &_id);
    glBindVertexArray(_id);
    glBindBuffer(GL_ARRAY_BUFFER, std::static_pointer_cast<gl::VertexBuffer>(_vertexBuffer)->getHandle());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, std::static_pointer_cast<gl::IndexBuffer>(_indexBuffer)->getHandle());
    LOG_DEBUG("Mesh", "gfx:: MESH CREATED $0", _id);
}

Mesh::~Mesh() {
    if (_id > 0)
        glDeleteVertexArrays(1, &_id);
}

void Mesh::draw() {
    glBindVertexArray(_id);
    glDrawElements(GL_TRIANGLES, _indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
}

} // namespace atta::graphics::gl
