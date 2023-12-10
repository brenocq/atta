//--------------------------------------------------
// Atta Graphics Module
// vertexBuffer.h
// Date: 2023-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_VERTEX_BUFFER_H
#define ATTA_GRAPHICS_APIS_VULKAN_VERTEX_BUFFER_H

#include <atta/graphics/apis/vulkan/buffer.h>
#include <atta/graphics/vertexBuffer.h>

namespace atta::graphics::vk {

class VertexBuffer final : public Buffer, public gfx::VertexBuffer {
  public:
    VertexBuffer(const gfx::VertexBuffer::CreateInfo& info);
    ~VertexBuffer();

    void bind() const override;
    void bind(VkCommandBuffer commandBuffer) const;

    static VkVertexInputBindingDescription getBindingDescription(const BufferLayout& layout);
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(const BufferLayout& layout);

  private:
    static VkFormat convertType(BufferLayout::Element::Type type);
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_VERTEX_BUFFER_H
