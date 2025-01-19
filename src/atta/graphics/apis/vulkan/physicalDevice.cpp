//--------------------------------------------------
// Atta Graphics Module
// physicalDevice.cpp
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/physicalDevice.h>
#include <atta/graphics/windows/glfwWindow.h>

namespace atta::graphics::vk {

const std::vector<const char*> PhysicalDevice::deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

PhysicalDevice::PhysicalDevice(std::shared_ptr<Instance> instance) : _instance(instance) {
    // Get device count
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance->getHandle(), &deviceCount, nullptr);

    // Check if some device was found
    if (deviceCount == 0)
        LOG_ERROR("gfx::vk::PhysicalDevice", "Failed to find GPUs with Vulkan support!");

    // Get physical devices
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_instance->getHandle(), &deviceCount, devices.data());
    PhysicalDevice::printPhysicalDevices(devices);

    // Choose physical device
    _physicalDevice = VK_NULL_HANDLE;
    for (VkPhysicalDevice device : devices)
        if (isDeviceSuitable(device)) {
            _physicalDevice = device;
            break;
        }

    // Check found physical device
    if (_physicalDevice == VK_NULL_HANDLE) {
        LOG_ERROR("gfx::vk::PhysicalDevice", "No suitable GPU found.");
        return;
    }

    // Query queues
    _queueFamilyIndices = findQueueFamilies(_physicalDevice);
    LOG_SUCCESS("gfx::vk::PhysicalDevice", "Found suitable GPU!");
}

PhysicalDevice::~PhysicalDevice() {}

VkPhysicalDevice PhysicalDevice::getHandle() const { return _physicalDevice; }

std::shared_ptr<Instance> PhysicalDevice::getInstance() const { return _instance; }

PhysicalDevice::QueueFamilyIndices PhysicalDevice::getQueueFamilyIndices() const { return _queueFamilyIndices; }

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && supportedFeatures.samplerAnisotropy;
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    // Get available extensions
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    // Check required extensions
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (const auto& extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

PhysicalDevice::QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices{};

    // Get queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // Find queue families
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        // Check queueFamily surface support (present support)
        VkBool32 presentSupport = glfwGetPhysicalDevicePresentationSupport(_instance->getHandle(), device, i);
        if (presentSupport)
            indices.presentFamily = i;

        // Find queue family dedicated to tranfer
        if ((int)(queueFamily.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT)) == (int)VK_QUEUE_TRANSFER_BIT)
            indices.transferFamily = i;

        if (indices.isComplete())
            break;

        i++;
    }

    return indices;
}

std::string getVersion(const uint32_t version) {
    // Convert version to human readable
    std::stringstream ss;
    ss << VK_VERSION_MAJOR(version);
    ss << ".";
    ss << VK_VERSION_MINOR(version);
    ss << ".";
    ss << VK_VERSION_PATCH(version);
    return ss.str();
}

std::string getVersion(const uint32_t version, const uint32_t vendorId) {
    // Convert version to human readable
    std::stringstream ss;
    ss << VK_VERSION_MAJOR(version);
    ss << ".";
    ss << (VK_VERSION_MINOR(version) >> (vendorId == 0x10DE ? 2 : 0));
    ss << ".";
    ss << (VK_VERSION_PATCH(version) >> (vendorId == 0x10DE ? 4 : 0));
    return ss.str();
}

void PhysicalDevice::printPhysicalDevices(std::vector<VkPhysicalDevice> physicalDevices) {
    bool showQueues = true;
    bool showFeatures = false;
    bool showLimits = false;
    bool showAvailableLayers = false;
    bool showAvailableExtensions = false;
    bool showMemory = false;
    bool showRayTracingInfo = false;

    LOG_INFO("gfx::vk::PhysicalDevice", "[w]$0 GPU$1 with Vulkan support", physicalDevices.size(), physicalDevices.size() > 1 ? "s" : "");
    for (VkPhysicalDevice device : physicalDevices) {
        VkPhysicalDeviceProperties prop;
        vkGetPhysicalDeviceProperties(device, &prop);

        std::string vendorId = "Unknown";
        switch (prop.vendorID) {
            case 0x1002:
                vendorId = "AMD";
                break;
            case 0x1010:
                vendorId = "ImgTec";
                break;
            case 0x10DE:
                vendorId = "NVIDIA";
                break;
            case 0x13B5:
                vendorId = "ARM";
                break;
            case 0x5143:
                vendorId = "Qualcomm";
                break;
            case 0x8086:
                vendorId = "Intel";
                break;
            default:
                break;
        }

        std::string deviceType = "Unknown";
        switch (prop.deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                deviceType = "Other";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                deviceType = "Integrated GPU";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                deviceType = "Discrete GPU";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                deviceType = "Virtual GPU";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                deviceType = "CPU";
            default:
                break;
        }

        std::cout << "- ";
        std::cout << COLOR_BOLD_CYAN << vendorId << " " << prop.deviceName << COLOR_WHITE;
        std::cout << " (";
        std::cout << deviceType << ": ";
        std::cout << "vulkan " << getVersion(prop.apiVersion) << ", ";
        std::cout << "driver " << getVersion(prop.driverVersion, prop.vendorID);
        std::cout << ")" << std::endl;

        if (showQueues) {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            std::cout << "  - " << COLOR_CYAN << "Queue families" << COLOR_WHITE << " (" << queueFamilyCount << "):" << std::endl;
            for (auto queueFamily : queueFamilies) {
                std::cout << "    - ";
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                    std::cout << "G";
                if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
                    std::cout << "C";
                if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
                    std::cout << "T";
                if (queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
                    std::cout << "S";
                if (queueFamily.queueFlags & VK_QUEUE_PROTECTED_BIT)
                    std::cout << "P";
                printf(" (%d queues)\n", queueFamily.queueCount);
            }
        }

        if (showLimits) {
            VkPhysicalDeviceLimits limits = prop.limits;
            std::cout << "  - " << COLOR_CYAN << "Limits" << COLOR_WHITE << std::endl;
            std::cout << "    - maxImageDimension1D: " << limits.maxImageDimension1D << " texels" << std::endl;
            std::cout << "    - maxImageDimension2D: " << limits.maxImageDimension2D << " texels" << std::endl;
            std::cout << "    - maxImageDimension3D: " << limits.maxImageDimension3D << " texels" << std::endl;
            std::cout << "    - maxTexelBufferElements: " << limits.maxTexelBufferElements << " texels (" << (int)sqrt(limits.maxTexelBufferElements)
                      << " x " << (int)sqrt(limits.maxTexelBufferElements) << ")" << std::endl;
            std::cout << "    - maxMemoryAllocationCount: " << limits.maxMemoryAllocationCount << " times" << std::endl;
        }

        if (showFeatures) {
            std::cout << COLOR_WHITE << "  - " << COLOR_CYAN << "Features" << COLOR_WHITE << std::endl;

            VkPhysicalDeviceFeatures features;
            vkGetPhysicalDeviceFeatures(device, &features);

            std::cout << "    - wideLines: " << (features.wideLines ? "OK" : "Does not support") << std::endl;
            std::cout << "    - largePoints: " << (features.largePoints ? "OK" : "Does not support") << std::endl;
            std::cout << "    - depthClamp: " << (features.depthClamp ? "OK" : "Does not support") << std::endl;
            std::cout << "    - compression BC: " << (features.textureCompressionBC ? "OK" : "Does not support") << std::endl;
            std::cout << "    - compression ETC2: " << (features.textureCompressionETC2 ? "OK" : "Does not support") << std::endl;
            std::cout << "    - compression ASTC_LDR: " << (features.textureCompressionASTC_LDR ? "OK" : "Does not support") << std::endl;
        }

        if (showAvailableLayers) {
            std::cout << "  - " << COLOR_CYAN << "Available layers" << COLOR_WHITE << std::endl;

            uint32_t propertyCount = 0;
            vkEnumerateDeviceLayerProperties(device, &propertyCount, nullptr);

            if (propertyCount == 0) {
                std::cout << COLOR_RED << "    There are no available layer properties!" << COLOR_WHITE << std::endl;
                return;
            }

            std::vector<VkLayerProperties> properties(propertyCount);
            vkEnumerateDeviceLayerProperties(device, &propertyCount, properties.data());

            for (const auto& property : properties) {
                std::cout << "    - " << property.layerName << " (" << property.description << ")" << std::endl;
            }

            std::cout << COLOR_WHITE;
        }

        if (showAvailableExtensions) {
            std::cout << "  - " << COLOR_CYAN << "Available extensions" << COLOR_WHITE << std::endl;

            uint32_t propertyCount = 0;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &propertyCount, nullptr);

            if (propertyCount == 0) {
                std::cout << COLOR_RED << "    There are no available extension properties!" << COLOR_WHITE << std::endl;
                return;
            }

            std::vector<VkExtensionProperties> properties(propertyCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &propertyCount, properties.data());

            for (const auto& property : properties) {
                std::cout << "    - " << property.extensionName << std::endl;
            }

            std::cout << COLOR_WHITE;
        }

        if (showMemory) {
            std::cout << "  - " << COLOR_CYAN << "Memory" << COLOR_WHITE << std::endl;

            VkPhysicalDeviceMemoryProperties memProp;
            vkGetPhysicalDeviceMemoryProperties(device, &memProp);
            // std::cout << memProp.memoryTypeCount << std::endl;
            std::cout << "    - heaps (" << memProp.memoryHeapCount << ")" << std::endl;
            for (int i = 0; i < (int)memProp.memoryHeapCount; i++)
                std::cout << "      - heap " << i << ": " << -int(memProp.memoryHeaps[i].size) / int(1 << 20) << "Mb" << std::endl;
        }

        if (showRayTracingInfo) {
            std::cout << "  - " << COLOR_CYAN << "Ray tracing KHR" << COLOR_WHITE << std::endl;

            VkPhysicalDeviceRayTracingPipelinePropertiesKHR rtProps = {};
            rtProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;

            VkPhysicalDeviceProperties2 props = {};
            props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
            props.pNext = &rtProps;
            vkGetPhysicalDeviceProperties2(device, &props);

            std::cout << "    - maxRecursionDepth: " << rtProps.maxRayRecursionDepth << std::endl;
            std::cout << "    - shaderGroupBaseAlignment: " << rtProps.shaderGroupBaseAlignment << std::endl;
        }
    }
}

} // namespace atta::graphics::vk
