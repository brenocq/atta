//--------------------------------------------------
// Atta Graphics Module
// surface.cpp
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/surface.h>

#include <atta/graphics/windows/glfwWindow.h>

namespace atta::graphics::vk {

Surface::Surface(std::shared_ptr<Instance> instance, std::shared_ptr<Window> window) : _instance(instance), _window(window) {
    GLFWwindow* w = glfwGetCurrentContext();
    int result = 0;
    if ((result = glfwCreateWindowSurface(_instance->getHandle(), w, nullptr, &_surface)) != VK_SUCCESS) {
        const char* description;
        glfwGetError(&description);
        LOG_ERROR("gfx::vk::Surface", "Failed to create window surface! Code: $0, message: $1", result, description);
    }
}

Surface::~Surface() { vkDestroySurfaceKHR(_instance->getHandle(), _surface, nullptr); }

VkSurfaceKHR Surface::getHandle() const { return _surface; }

std::shared_ptr<Window> Surface::getWindow() const { return _window; }

} // namespace atta::graphics::vk
