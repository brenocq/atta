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
#include <atta/graphics/windows/window.h>

namespace atta::graphics::vk {

class Surface {
  public:
    Surface(std::shared_ptr<Instance> instance, std::shared_ptr<Window> window);
    ~Surface();

    VkSurfaceKHR getHandle() const;
    std::shared_ptr<Window> getWindow() const;

  private:
    VkSurfaceKHR _surface;
    std::shared_ptr<Instance> _instance;
    std::shared_ptr<Window> _window;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_SURFACE_H
