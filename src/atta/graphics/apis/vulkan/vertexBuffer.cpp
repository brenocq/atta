//--------------------------------------------------
// Atta Graphics Module
// vertexBuffer.cpp
// Date: 2023-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/vertexBuffer.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>

namespace atta::graphics::vk {

VertexBuffer::VertexBuffer(const graphics::VertexBuffer::CreateInfo& info) : graphics::VertexBuffer(info), _device(common::getDevice()) {
    // Create vertex buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = _size;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(_device->getHandle(), &bufferInfo, nullptr, &_vertexBuffer) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::VertexBuffer", "Failed to create vertex buffer");

    // Get memory requirements
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device->getHandle(), _vertexBuffer, &memRequirements);

    // Find memory
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
        findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // Allocate memory
    if (vkAllocateMemory(_device->getHandle(), &allocInfo, nullptr, &_memory) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::VertexBuffer", "Failed to allocate vertex buffer memory");

    // Bind memory to vertex buffer
    vkBindBufferMemory(_device->getHandle(), _vertexBuffer, _memory, 0);

    // Copy data from CPU to GPU
    if (_data) {
        void* gpuData;
        vkMapMemory(_device->getHandle(), _memory, 0, _size, 0, &gpuData);
        memcpy(gpuData, _data, (size_t)_size);
        vkUnmapMemory(_device->getHandle(), _memory);
    }
}

VertexBuffer::~VertexBuffer() {
    if (_vertexBuffer != VK_NULL_HANDLE)
        vkDestroyBuffer(_device->getHandle(), _vertexBuffer, nullptr);
    if (_memory != VK_NULL_HANDLE)
        vkFreeMemory(_device->getHandle(), _memory, nullptr);
}

void VertexBuffer::bind() const {
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(common::getCommandBuffers()->getCurrent(), 0, 1, &_vertexBuffer, offsets);
}

VkBuffer VertexBuffer::getHandle() const { return _vertexBuffer; }

VkDeviceMemory VertexBuffer::getMemoryHandle() const { return _memory; }

VkVertexInputBindingDescription VertexBuffer::getBindingDescription(const VertexBufferLayout& layout) {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = layout.getStride();
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> VertexBuffer::getAttributeDescriptions(const VertexBufferLayout& layout) {
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

VkFormat VertexBuffer::convertType(VertexBufferElement::Type type) {
    switch (type) {
        case VertexBufferElement::Type::BOOL:
        case VertexBufferElement::Type::INT:
            return VK_FORMAT_R32_SINT;
        case VertexBufferElement::Type::UINT:
            return VK_FORMAT_R32_UINT;
        case VertexBufferElement::Type::IVEC2:
            return VK_FORMAT_R32G32_SINT;
        case VertexBufferElement::Type::IVEC3:
            return VK_FORMAT_R32G32B32_SINT;
        case VertexBufferElement::Type::IVEC4:
            return VK_FORMAT_R32G32B32A32_SINT;
        case VertexBufferElement::Type::FLOAT:
            return VK_FORMAT_R32_SFLOAT;
        case VertexBufferElement::Type::VEC2:
            return VK_FORMAT_R32G32_SFLOAT;
        case VertexBufferElement::Type::VEC3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case VertexBufferElement::Type::VEC4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case VertexBufferElement::Type::MAT3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case VertexBufferElement::Type::MAT4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            LOG_ERROR("gfx::vk::VertexBuffer", "Unknown vertex buffer element type");
            return VK_FORMAT_UNDEFINED;
    }
}

uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_device->getPhysicalDevice()->getHandle(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;

    LOG_ERROR("gfx::vk::VertexBuffer", "Failed to find suitable memory type");
    return 0;
}

} // namespace atta::graphics::vk
