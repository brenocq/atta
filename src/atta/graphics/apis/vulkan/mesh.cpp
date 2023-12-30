//--------------------------------------------------
// Atta Graphics Module
// mesh.cpp
// Date: 2023-04-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/mesh.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>
#include <atta/graphics/apis/vulkan/indexBuffer.h>
#include <atta/graphics/apis/vulkan/stagingBuffer.h>
#include <atta/graphics/apis/vulkan/vertexBuffer.h>
#include <atta/graphics/interface.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

namespace atta::graphics::vk {

Mesh::Mesh(CreateInfo info) : gfx::Mesh(info) {
    _vertexBuffer = gfx::create<gfx::VertexBuffer>(info.vertexBufferInfo);
    if (info.indexBufferInfo.size > 0)
        _indexBuffer = gfx::create<gfx::IndexBuffer>(info.indexBufferInfo);
}

Mesh::~Mesh() {}

void Mesh::draw() {}

void Mesh::draw(VkCommandBuffer commandBuffer) {
    std::dynamic_pointer_cast<vk::VertexBuffer>(_vertexBuffer)->bind(commandBuffer);
    std::dynamic_pointer_cast<vk::IndexBuffer>(_indexBuffer)->bind(commandBuffer);
    if (_indexBuffer)
        vkCmdDrawIndexed(commandBuffer, _indexBuffer->getCount(), 1, 0, 0, 0);
    else
        vkCmdDraw(commandBuffer, _vertexBuffer->getCount(), 1, 0, 0);
}

} // namespace atta::graphics::vk
