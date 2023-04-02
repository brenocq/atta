//--------------------------------------------------
// Atta Graphics Module
// debugMessenger.h
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_DEBUG_MESSENGER_H
#define ATTA_GRAPHICS_APIS_VULKAN_DEBUG_MESSENGER_H
#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/instance.h>

namespace atta::graphics::vk {

class DebugMessenger {
  public:
    DebugMessenger(std::shared_ptr<vk::Instance> instance);
    ~DebugMessenger();

    static VKAPI_ATTR VkBool32 VKAPI_CALL callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                   VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

  private:
    VkDebugUtilsMessengerEXT _debugMessenger;
    std::shared_ptr<vk::Instance> _instance;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_DEBUG_MESSENGER_H
