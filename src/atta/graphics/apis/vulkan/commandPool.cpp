//--------------------------------------------------
// Atta Graphics Module
// commandPool.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/commandPool.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>

namespace atta::graphics::vk {

CommandPool::CommandPool(std::shared_ptr<Device> device, DeviceQueueFamily deviceQueueFamily) : _device(device) {
    std::shared_ptr<PhysicalDevice> physicalDevice = _device->getPhysicalDevice();
    PhysicalDevice::QueueFamilyIndices queueFamilyIndices = physicalDevice->getQueueFamilyIndices();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    // Select queue
    switch (_deviceQueueFamily) {
        case DEVICE_QUEUE_FAMILY_GRAPHICS:
            poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
            break;
        case DEVICE_QUEUE_FAMILY_TRANSFER:
            poolInfo.queueFamilyIndex = queueFamilyIndices.transferFamily.value();
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

} // namespace atta::graphics::vk
