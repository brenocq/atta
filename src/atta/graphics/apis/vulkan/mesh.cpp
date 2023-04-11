//--------------------------------------------------
// Atta Graphics Module
// mesh.cpp
// Date: 2023-04-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/mesh.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

namespace atta::graphics::vk {

Mesh::Mesh(StringId sid) : gfx::Mesh(sid) {
    resource::Mesh* mesh = resource::get<resource::Mesh>(sid.getString());

    gfx::VertexBuffer::CreateInfo vertexInfo{};
    vertexInfo.data = reinterpret_cast<const uint8_t*>(mesh->getVertices().data());
    vertexInfo.size = mesh->getVertices().size() * sizeof(mesh->getVertices()[0]);
    vertexInfo.layout = {{"inPosition", VertexBufferElement::Type::VEC3},
                         {"inNormal", VertexBufferElement::Type::VEC3},
                         {"inTexCoord", VertexBufferElement::Type::VEC2}};
    _vertexBuffer = std::make_shared<VertexBuffer>(vertexInfo);

    gfx::IndexBuffer::CreateInfo indexInfo{};
    indexInfo.data = reinterpret_cast<const uint8_t*>(mesh->getIndices().data());
    indexInfo.size = mesh->getIndices().size() * sizeof(mesh->getIndices()[0]);
    _indexBuffer = std::make_shared<IndexBuffer>(indexInfo);
}

Mesh::~Mesh() {}

void Mesh::draw() {
    _vertexBuffer->bind();
    _indexBuffer->bind();
    vkCmdDrawIndexed(common::getCommandBuffers()->getCurrent(), _indexBuffer->getCount(), 1, 0, 0, 0);
}

} // namespace atta::graphics::vk
