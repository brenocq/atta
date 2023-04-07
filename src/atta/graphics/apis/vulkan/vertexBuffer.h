//--------------------------------------------------
// Atta Graphics Module
// vertexBuffer.h
// Date: 2023-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_VERTEX_BUFFER_H
#define ATTA_GRAPHICS_APIS_VULKAN_VERTEX_BUFFER_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/vertexBuffer.h>

namespace atta::graphics::vk {

class VertexBuffer final : public graphics::VertexBuffer {
  public:
    VertexBuffer(const graphics::VertexBuffer::CreateInfo& info);
    ~VertexBuffer();

    void bind() const override;

    VkBuffer getHandle() const;
    VkDeviceMemory getMemoryHandle() const;

    static VkVertexInputBindingDescription getBindingDescription(const VertexBufferLayout& layout);
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(const VertexBufferLayout& layout);

  private:
    static VkFormat convertType(VertexBufferElement::Type type);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkBuffer _vertexBuffer;
    VkDeviceMemory _memory;
    std::shared_ptr<Device> _device;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_VERTEX_BUFFER_H
