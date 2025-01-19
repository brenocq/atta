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
    int result = 0;
    if ((result = glfwCreateWindowSurface(_instance->getHandle(), (GLFWwindow*)window->getHandle(), nullptr, &_surface)) != VK_SUCCESS) {
        const char* description;
        glfwGetError(&description);
        LOG_ERROR("gfx::vk::Surface", "Failed to create window surface! Code: $0, message: $1", result, description);
    }
}

Surface::~Surface() {
    if (_surface != VK_NULL_HANDLE)
        vkDestroySurfaceKHR(_instance->getHandle(), _surface, nullptr);
}

VkSurfaceKHR Surface::getHandle() const { return _surface; }

std::shared_ptr<Window> Surface::getWindow() const { return _window; }

} // namespace atta::graphics::vk
