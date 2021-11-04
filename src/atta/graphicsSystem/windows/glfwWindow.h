//--------------------------------------------------
// Atta Graphics System
// glfwWindow.h
// Date: 2021-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_WINDOWS_GLFW_WINDOW_H
#define ATTA_GRAPHICS_SYSTEM_WINDOWS_GLFW_WINDOW_H
//#if defined(ATTA_OS_LINUX) || defined(ATTA_OS_MACOS) || defined(ATTA_OS_WINDOWS)
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <atta/graphicsSystem/windows/window.h>

namespace atta
{
    class GlfwWindow final : public Window
    {
    public:
        GlfwWindow(const Window::CreateInfo& info);
        ~GlfwWindow();

        void update() override;
        void swapBuffers() override;

    private:
        GLFWwindow* _window;
        static unsigned _glfwWindowCounter;
    };
}

//#endif// ATTA_OS_XXX
#endif// ATTA_GRAPHICS_SYSTEM_WINDOWS_GLFW_WINDOW_H