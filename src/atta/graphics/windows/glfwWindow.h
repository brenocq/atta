// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
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
