//--------------------------------------------------
// Atta Graphics Module
// glfwWindow.h
// Date: 2021-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_WINDOWS_GLFW_WINDOW_H
#define ATTA_GRAPHICS_WINDOWS_GLFW_WINDOW_H
// #if defined(ATTA_OS_LINUX) || defined(ATTA_OS_MACOS) || defined(ATTA_OS_WINDOWS)

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <atta/graphics/windows/window.h>

namespace atta::graphics {

class GlfwWindow final : public Window {
  public:
    GlfwWindow(const Window::CreateInfo& info);
    ~GlfwWindow();

    void* getHandle() const override;

    void update() override;
    void swapBuffers() override;

  private:
    GLFWwindow* _window;
    static unsigned _glfwWindowCounter;
};

} // namespace atta::graphics

// #endif// ATTA_OS_XXX
#endif // ATTA_GRAPHICS_WINDOWS_GLFW_WINDOW_H
