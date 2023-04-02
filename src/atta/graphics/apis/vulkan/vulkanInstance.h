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

  private:
    void printAvailableExtensions();
    void printAvailableLayers();

    std::vector<const char*> getRequiredExtensions();

    VkInstance _instance;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_INSTANCE_H
