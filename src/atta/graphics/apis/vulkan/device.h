//--------------------------------------------------
// Atta Graphics Module
// device.h
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_DEVICE_H
#define ATTA_GRAPHICS_APIS_VULKAN_DEVICE_H
#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/physicalDevice.h>

namespace atta::graphics::vk {

class Device {
  public:
    Device(std::shared_ptr<PhysicalDevice> physicalDevice);
    ~Device();

    VkDevice getHandle() const;
    std::shared_ptr<PhysicalDevice> getPhysicalDevice() const;
    VkQueue getPresentQueue() const;
    VkQueue getGraphicsQueue() const;
    VkQueue getTransferQueue() const;
    VkSampleCountFlagBits getMsaaSamples() const;

  private:
    VkSampleCountFlagBits getMaxUsableSampleCount();

    VkDevice _device;
    std::shared_ptr<PhysicalDevice> _physicalDevice;
    VkQueue _presentQueue;
    VkQueue _graphicsQueue;
    VkQueue _transferQueue;
    VkSampleCountFlagBits _msaaSamples;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_DEVICE_H
