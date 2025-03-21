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
    if (info.indexBufferInfo.size > 0)
        _indexBuffer = gfx::create<gfx::IndexBuffer>(info.indexBufferInfo);
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (_id > 0)
        glDeleteVertexArrays(1, &_id);
}

void Mesh::draw(Pipeline::Primitive primitive, size_t numVertices) {
    glBindVertexArray(_id);
    if (_indexBuffer)
        glDrawElements(convert(primitive), _indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(convert(primitive), 0, numVertices == 0 ? _vertexBuffer->getCount() : numVertices);
    glBindVertexArray(0);
}

GLenum Mesh::convert(Pipeline::Primitive primitive) {
    switch (primitive) {
        case Pipeline::Primitive::POINT:
            return GL_POINTS;
        case Pipeline::Primitive::LINE:
            return GL_LINES;
        case Pipeline::Primitive::TRIANGLE:
            return GL_TRIANGLES;
    }
    return 0;
}

} // namespace atta::graphics::gl
