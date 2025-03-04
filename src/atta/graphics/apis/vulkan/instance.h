//--------------------------------------------------
// Atta Graphics Module
// vulkanInstance.h
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_INSTANCE_H
#define ATTA_GRAPHICS_APIS_VULKAN_INSTANCE_H

#include <atta/graphics/apis/vulkan/common.h>

namespace atta::graphics::vk {

class Instance {
  public:
    Instance();
    ~Instance();

    bool wasCreated() const;

    VkInstance getHandle() const;
    uint32_t getApiVersion() const;

  private:
    void printAvailableExtensions();
    void printAvailableLayers();

    std::vector<VkExtensionProperties> getAvailableExtensions() const;
    std::vector<VkLayerProperties> getAvailableLayers() const;

    std::vector<const char*> getEnabledExtensions();
    std::vector<const char*> getEnabledLayers();

    VkInstance _instance;
    bool _wasCreated;
    VkDebugUtilsMessengerEXT debugMessenger;
    uint32_t _apiVersion;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_INSTANCE_H
