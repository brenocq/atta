//--------------------------------------------------
// Atta Graphics Module
// commandPool.h
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_COMMAND_POOL_H
#define ATTA_GRAPHICS_APIS_VULKAN_COMMAND_POOL_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class CommandPool {
  public:
    enum DeviceQueueFamily {
        DEVICE_QUEUE_FAMILY_GRAPHICS = 0,
        DEVICE_QUEUE_FAMILY_TRANSFER,
    };

    CommandPool(std::shared_ptr<Device> device, DeviceQueueFamily deviceQueueFamily = DEVICE_QUEUE_FAMILY_GRAPHICS);
    ~CommandPool();

    VkCommandPool getHandle() const;
    std::shared_ptr<Device> getDevice() const;

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

  private:
    VkCommandPool _commandPool;
    std::shared_ptr<Device> _device;

    DeviceQueueFamily _deviceQueueFamily;
    VkQueue _submitQueue;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_COMMAND_POOL_H
