// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

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
