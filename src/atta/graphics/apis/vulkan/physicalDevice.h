//--------------------------------------------------
// Atta Graphics Module
// physicalDevice.h
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_PHYSICAL_DEVICE_H
#define ATTA_GRAPHICS_APIS_VULKAN_PHYSICAL_DEVICE_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/instance.h>

namespace atta::graphics::vk {

class PhysicalDevice {
  public:
    static const std::vector<const char*> deviceExtensions;

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        std::optional<uint32_t> transferFamily;
        bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value(); }
    };

    PhysicalDevice(std::shared_ptr<Instance> instance);
    ~PhysicalDevice();

    VkPhysicalDevice getHandle() const;
    std::shared_ptr<Instance> getInstance() const;
    QueueFamilyIndices getQueueFamilyIndices() const;

  private:
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    static void printPhysicalDevices(std::vector<VkPhysicalDevice> physicalDevices);

    VkPhysicalDevice _physicalDevice;
    std::shared_ptr<Instance> _instance;
    QueueFamilyIndices _queueFamilyIndices;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_PHYSICAL_DEVICE_H
