//--------------------------------------------------
// Atta Graphics Module
// surface.cpp
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/surface.h>

#include <atta/graphics/windows/glfwWindow.h>

namespace atta::graphics::vk {

Surface::Surface(std::shared_ptr<vk::Instance> instance) : _instance(instance) {
    GLFWwindow* window = glfwGetCurrentContext();
    int result = 0;
    if ((result = glfwCreateWindowSurface(_instance->getHandle(), window, nullptr, &_surface)) != VK_SUCCESS) {
        const char* description;
        glfwGetError(&description);
        LOG_ERROR("gfx::vk::Surface", "Failed to create window surface! Code: $0, message: $1", result, description);
    }
}

Surface::~Surface() { vkDestroySurfaceKHR(_instance->getHandle(), _surface, nullptr); }

VkSurfaceKHR Surface::getHandle() const { return _surface; }

} // namespace atta::graphics::vk
