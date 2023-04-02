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

    VkInstance get() const;

  private:
    void printAvailableExtensions();
    void printAvailableLayers();

    std::vector<const char*> getEnabledExtensions();
    std::vector<const char*> getEnabledLayers();

    VkInstance _instance;
    VkDebugUtilsMessengerEXT debugMessenger;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_INSTANCE_H
