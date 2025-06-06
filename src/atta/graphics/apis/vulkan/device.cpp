//--------------------------------------------------
// Atta Graphics Module
// device.cpp
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

Device::Device(std::shared_ptr<PhysicalDevice> physicalDevice) : _physicalDevice(physicalDevice) {
    _msaaSamples = getMaxUsableSampleCount();

    //----- Get queues -----//
    PhysicalDevice::QueueFamilyIndices indices = _physicalDevice->getQueueFamilyIndices();

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value(), indices.transferFamily.value()};
    _presentQueueFamily = indices.presentFamily.value();
    _graphicsQueueFamily = indices.graphicsFamily.value();
    _transferQueueFamily = indices.transferFamily.value();

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.flags = 0;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    //----- Get features -----//
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    //---------- Create logical device ----------//
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    //----- Defines queues -----//
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    //----- Enable features -----//
    createInfo.pEnabledFeatures = &deviceFeatures;

    //----- Enable extensions -----//
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(PhysicalDevice::deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = PhysicalDevice::deviceExtensions.data();

    //----- Create -----//
    if (vkCreateDevice(_physicalDevice->getHandle(), &createInfo, nullptr, &_device) != VK_SUCCESS) {
        LOG_ERROR("gfx::vk::Device", "Failed to create device!");
        return;
    }

    vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);
    vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
    vkGetDeviceQueue(_device, indices.transferFamily.value(), 0, &_transferQueue);
}

Device::~Device() {
    if (_device != VK_NULL_HANDLE)
        vkDestroyDevice(_device, nullptr);
}

VkDevice Device::getHandle() const { return _device; }

std::shared_ptr<PhysicalDevice> Device::getPhysicalDevice() const { return _physicalDevice; }

uint32_t Device::getPresentQueueFamily() const { return _presentQueueFamily; }

uint32_t Device::getGraphicsQueueFamily() const { return _graphicsQueueFamily; }

uint32_t Device::getTransferQueueFamily() const { return _transferQueueFamily; }

VkQueue Device::getPresentQueue() const { return _presentQueue; }

VkQueue Device::getGraphicsQueue() const { return _graphicsQueue; }

VkQueue Device::getTransferQueue() const { return _transferQueue; }

VkSampleCountFlagBits Device::getMsaaSamples() const { return _msaaSamples; }

VkSampleCountFlagBits Device::getMaxUsableSampleCount() {
    // Get number of samples supported
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(_physicalDevice->getHandle(), &physicalDeviceProperties);
    VkSampleCountFlags counts =
        physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

    // Return maximum samples
    if (counts & VK_SAMPLE_COUNT_64_BIT)
        return VK_SAMPLE_COUNT_64_BIT;
    else if (counts & VK_SAMPLE_COUNT_32_BIT)
        return VK_SAMPLE_COUNT_32_BIT;
    else if (counts & VK_SAMPLE_COUNT_16_BIT)
        return VK_SAMPLE_COUNT_16_BIT;
    else if (counts & VK_SAMPLE_COUNT_8_BIT)
        return VK_SAMPLE_COUNT_8_BIT;
    else if (counts & VK_SAMPLE_COUNT_4_BIT)
        return VK_SAMPLE_COUNT_4_BIT;
    else if (counts & VK_SAMPLE_COUNT_2_BIT)
        return VK_SAMPLE_COUNT_2_BIT;
    return VK_SAMPLE_COUNT_1_BIT;
}

} // namespace atta::graphics::vk
