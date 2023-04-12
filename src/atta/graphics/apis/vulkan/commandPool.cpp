//--------------------------------------------------
// Atta Graphics Module
// commandPool.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/commandPool.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>

namespace atta::graphics::vk {

CommandPool::CommandPool(std::shared_ptr<Device> device, DeviceQueueFamily deviceQueueFamily)
    : _device(device), _deviceQueueFamily(deviceQueueFamily) {
    std::shared_ptr<PhysicalDevice> physicalDevice = _device->getPhysicalDevice();
    PhysicalDevice::QueueFamilyIndices queueFamilyIndices = physicalDevice->getQueueFamilyIndices();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    // Select queue
    switch (_deviceQueueFamily) {
        case DEVICE_QUEUE_FAMILY_GRAPHICS:
            poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
            _submitQueue = _device->getGraphicsQueue();
            break;
        case DEVICE_QUEUE_FAMILY_TRANSFER:
            poolInfo.queueFamilyIndex = queueFamilyIndices.transferFamily.value();
            _submitQueue = _device->getTransferQueue();
            break;
        default:
            break;
    }

    // Create command pool
    if (vkCreateCommandPool(_device->getHandle(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::CommandPool", "Failed to create command pool!");
}

CommandPool::~CommandPool() {
    if (_commandPool != VK_NULL_HANDLE)
        vkDestroyCommandPool(_device->getHandle(), _commandPool, nullptr);
}

VkCommandPool CommandPool::getHandle() const { return _commandPool; }
std::shared_ptr<Device> CommandPool::getDevice() const { return _device; }

VkCommandBuffer CommandPool::beginSingleTimeCommands() {
    // Allocation is slow... maybe use some command buffer already in the command pool
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = _commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device->getHandle(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void CommandPool::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(_submitQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_submitQueue);

    vkFreeCommandBuffers(_device->getHandle(), _commandPool, 1, &commandBuffer);
}

} // namespace atta::graphics::vk
