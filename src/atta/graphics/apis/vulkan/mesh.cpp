//--------------------------------------------------
// Atta Graphics Module
// mesh.cpp
// Date: 2023-04-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/mesh.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>
#include <atta/graphics/apis/vulkan/stagingBuffer.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

namespace atta::graphics::vk {

Mesh::Mesh(CreateInfo info) : gfx::Mesh(info) {}

Mesh::~Mesh() {}

void Mesh::draw() {
    _vertexBuffer->bind();
    _indexBuffer->bind();
    vkCmdDrawIndexed(common::getCommandBuffers()->getCurrent(), _indexBuffer->getCount(), 1, 0, 0, 0);
}

} // namespace atta::graphics::vk
