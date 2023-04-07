//--------------------------------------------------
// Atta Graphics Module
// buffer.cpp
// Date: 2023-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/buffer.h>

#include <atta/graphics/apis/vulkan/commandPool.h>

namespace atta::graphics::vk {

Buffer::Buffer(const CreateInfo& info) : _bufferSize(info.size), _device(common::getDevice()) {
    // Create buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = _bufferSize;
    bufferInfo.usage = info.usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(_device->getHandle(), &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Buffer", "Failed to create buffer");

    // Get memory requirements
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device->getHandle(), _buffer, &memRequirements);

    // Find memory
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, info.properties);

    // Allocate memory
    if (vkAllocateMemory(_device->getHandle(), &allocInfo, nullptr, &_memory) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Buffer", "Failed to allocate buffer memory");

    // Bind memory to buffer
    vkBindBufferMemory(_device->getHandle(), _buffer, _memory, 0);
}

Buffer::~Buffer() {
    if (_buffer != VK_NULL_HANDLE)
        vkDestroyBuffer(_device->getHandle(), _buffer, nullptr);
    if (_memory != VK_NULL_HANDLE)
        vkFreeMemory(_device->getHandle(), _memory, nullptr);
}

VkBuffer Buffer::getHandle() const { return _buffer; }

VkDeviceMemory Buffer::getMemoryHandle() const { return _memory; }

uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_device->getPhysicalDevice()->getHandle(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;

    LOG_ERROR("gfx::vk::Buffer", "Failed to find suitable memory type");
    return 0;
}

void Buffer::copy(std::shared_ptr<Buffer> src, std::shared_ptr<Buffer> dst) {
    if (src->_bufferSize != dst->_bufferSize)
        LOG_WARN("gfx::vk::Buffer", "Trying to copy from buffer of size [w]$0[] to buffer of size [w]$1[], but buffers should have the same size",
                 src->_bufferSize, dst->_bufferSize);

    VkCommandBuffer commandBuffer = common::getCommandPool()->beginSingleTimeCommands();
    {
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = src->_bufferSize;
        vkCmdCopyBuffer(commandBuffer, src->getHandle(), dst->getHandle(), 1, &copyRegion);
    }
    common::getCommandPool()->endSingleTimeCommands(commandBuffer);
}

} // namespace atta::graphics::vk
