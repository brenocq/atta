//--------------------------------------------------
// Atta Graphics Module
// indexBuffer.cpp
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/indexBuffer.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>

namespace atta::graphics::vk {

IndexBuffer::IndexBuffer(const graphics::IndexBuffer::CreateInfo& info)
    : Buffer({
          info.size,
          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      }),
      graphics::IndexBuffer(info) {}

IndexBuffer::~IndexBuffer() {}

void IndexBuffer::bind() const { vkCmdBindIndexBuffer(common::getCommandBuffers()->getCurrent(), _buffer, 0, VK_INDEX_TYPE_UINT32); }

} // namespace atta::graphics::vk
