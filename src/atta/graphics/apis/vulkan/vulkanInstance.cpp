//--------------------------------------------------
// Atta Graphics Module
// vulkanInstance.cpp
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/vulkanInstance.h>

#include <atta/graphics/windows/glfwWindow.h>

namespace atta::graphics::vk {

Instance::Instance() {
    // Application info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Atta";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    // Create instance info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Setup required extensions
    std::vector<const char*> extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
#ifdef ATTA_OS_MACOS
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    // Setup validation layers
    if (false) {
        const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    printAvailableExtensions();
    printAvailableLayers();

    VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);
    if (result != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Instance", "Failed to create vulkan instance! Code:$0", toString(result));
    else
        LOG_SUCCESS("gfx::vk::Instance", "Created");
}

Instance::~Instance() {}

void Instance::printAvailableExtensions() {
    LOG_INFO("gfx::vk::Instance", "Available instance extensions:");

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    for (const auto& extension : extensions)
        LOG_INFO("gfx::vk::Instance", " - $0", extension.extensionName);
}

void Instance::printAvailableLayers() {
    uint32_t propertyCount = 0;
    vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
    std::vector<VkLayerProperties> properties(propertyCount);
    vkEnumerateInstanceLayerProperties(&propertyCount, properties.data());

    LOG_INFO("gfx::vk::Instance", "Available instance layers:");
    for (const auto& property : properties)
        LOG_INFO("gfx::vk::Instance", " - $0 ($1)", property.layerName, property.description);
}

std::vector<const char*> Instance::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef ATTA_OS_MACOS
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif
    // extensions.push_back(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);
    // extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

} // namespace atta::graphics::vk
