//--------------------------------------------------
// Atta Graphics Module
// surface.h
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_SURFACE_H
#define ATTA_GRAPHICS_APIS_VULKAN_SURFACE_H
#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/instance.h>

namespace atta::graphics::vk {

class Surface {
  public:
    Surface(std::shared_ptr<vk::Instance> instance);
    ~Surface();

    VkSurfaceKHR getHandle() const;

  private:
    VkSurfaceKHR _surface;
    std::shared_ptr<vk::Instance> _instance;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_SURFACE_H
