//--------------------------------------------------
// Atta Graphics Module
// vertexBuffer.cpp
// Date: 2023-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/vertexBuffer.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>

namespace atta::graphics::vk {

VertexBuffer::VertexBuffer(const gfx::VertexBuffer::CreateInfo& info)
    : Buffer({
          info.size,
          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      }),
      gfx::VertexBuffer(info) {
    LOG_DEBUG("gfx::vk::VertexBuffer", "Create vertex buffer $0", info.size);
    if (info.data)
        Buffer::setData(info.data);
}

VertexBuffer::~VertexBuffer() {}

void VertexBuffer::bind() const {}

void VertexBuffer::bind(VkCommandBuffer commandBuffer) const {
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &_buffer, offsets);
}

VkVertexInputBindingDescription VertexBuffer::getBindingDescription(const BufferLayout& layout) {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = layout.getStride();
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> VertexBuffer::getAttributeDescriptions(const BufferLayout& layout) {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    for (const auto& element : layout.getElements()) {
        VkVertexInputAttributeDescription attributeDescription{};
        attributeDescription.binding = 0;
        attributeDescription.location = attributeDescriptions.size();
        attributeDescription.format = convertType(element.type);
        attributeDescription.offset = element.offset;
        attributeDescriptions.push_back(attributeDescription);
    }
    return attributeDescriptions;
}

VkFormat VertexBuffer::convertType(BufferLayout::Element::Type type) {
    switch (type) {
        case BufferLayout::Element::Type::BOOL:
        case BufferLayout::Element::Type::INT:
            return VK_FORMAT_R32_SINT;
        case BufferLayout::Element::Type::UINT:
            return VK_FORMAT_R32_UINT;
        case BufferLayout::Element::Type::IVEC2:
            return VK_FORMAT_R32G32_SINT;
        case BufferLayout::Element::Type::IVEC3:
            return VK_FORMAT_R32G32B32_SINT;
        case BufferLayout::Element::Type::IVEC4:
            return VK_FORMAT_R32G32B32A32_SINT;
        case BufferLayout::Element::Type::FLOAT:
            return VK_FORMAT_R32_SFLOAT;
        case BufferLayout::Element::Type::VEC2:
            return VK_FORMAT_R32G32_SFLOAT;
        case BufferLayout::Element::Type::VEC3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case BufferLayout::Element::Type::VEC4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case BufferLayout::Element::Type::MAT3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case BufferLayout::Element::Type::MAT4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            LOG_ERROR("gfx::vk::VertexBuffer", "Unknown vertex buffer element type");
            return VK_FORMAT_UNDEFINED;
    }
}

} // namespace atta::graphics::vk
