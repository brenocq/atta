//--------------------------------------------------
// Atta Graphics Module
// openGLMesh.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/openGLMesh.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

namespace atta::graphics {

OpenGLMesh::OpenGLMesh(StringId sid) : _sid(sid), _id(0) {
    resource::Mesh* mesh = resource::get<resource::Mesh>(sid.getString());

    glGenVertexArrays(1, &_id);
    glBindVertexArray(_id);

    VertexBuffer::CreateInfo vertexInfo{};
    vertexInfo.data = reinterpret_cast<const uint8_t*>(mesh->getVertices().data());
    vertexInfo.size = mesh->getVertices().size() * sizeof(mesh->getVertices()[0]);
    vertexInfo.layout = {{"inPosition", VertexBufferElement::Type::VEC3},
                         {"inNormal", VertexBufferElement::Type::VEC3},
                         {"inTexCoord", VertexBufferElement::Type::VEC2}};
    _vertexBuffer = std::make_shared<OpenGLVertexBuffer>(vertexInfo);

    IndexBuffer::CreateInfo indexInfo{};
    indexInfo.data = reinterpret_cast<const uint8_t*>(mesh->getIndices().data());
    indexInfo.size = mesh->getIndices().size() * sizeof(mesh->getIndices()[0]);
    _indexBuffer = std::make_shared<OpenGLIndexBuffer>(indexInfo);
}

OpenGLMesh::~OpenGLMesh() {
    if (_id > 0)
        glDeleteVertexArrays(1, &_id);
}

void OpenGLMesh::draw() {
    glBindVertexArray(_id);
    glDrawElements(GL_TRIANGLES, _indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
}

} // namespace atta::graphics
