//--------------------------------------------------
// Atta Graphics Module
// glfwWindow.cpp
// Date: 2021-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/events/windowClose.h>
#include <atta/event/events/windowFocus.h>
#include <atta/event/events/windowKeyboardButton.h>
#include <atta/event/events/windowMouseButton.h>
#include <atta/event/events/windowMouseMove.h>
#include <atta/event/events/windowMouseScroll.h>
#include <atta/event/events/windowResize.h>
#include <atta/graphics/windows/glfwWindow.h>

#ifdef ATTA_OS_WEB
#include <emscripten.h>
EM_JS(int, canvas_get_width, (), { return Module.canvas.width; });
EM_JS(int, canvas_get_height, (), { return Module.canvas.height; });
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

namespace atta::graphics {

unsigned GlfwWindow::_glfwWindowCounter = 0;

GlfwWindow::GlfwWindow(const CreateInfo& info) : Window(info) {
    if (_glfwWindowCounter++ == 0) // XXX
        glfwInit();

    if (info.useOpenGL) {
#if defined(ATTA_OS_MACOS)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    } else
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(_window, (void*)this);

    glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
        event::WindowClose e;
        event::publish(e);
    });

    glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
        GlfwWindow& w = *(GlfwWindow*)glfwGetWindowUserPointer(window);
        w._width = width;
        w._height = height;
        event::WindowResize e((size_t)width, (size_t)height);
        event::publish(e);
    });

    glfwSetCursorEnterCallback(_window, [](GLFWwindow* window, int entered) {
        event::WindowFocus e(entered != 0);
        event::publish(e);
    });

    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
        event::WindowMouseMove e((float)xPos, (float)yPos);
        event::publish(e);
    });

    glfwSetScrollCallback(_window, [](GLFWwindow* window, double dx, double dy) {
        event::WindowMouseScroll e((float)dx, (float)dy);
        event::publish(e);
    });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            event::WindowMouseButton e(button, event::WindowMouseButton::Action::PRESS);
            event::publish(e);
        } else if (action == GLFW_RELEASE) {
            event::WindowMouseButton e(button, event::WindowMouseButton::Action::RELEASE);
            event::publish(e);
        }
    });

    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        event::WindowKeyboardButton::Action a;
        switch (action) {
            case GLFW_PRESS:
                a = event::WindowKeyboardButton::Action::PRESS;
                break;
            case GLFW_REPEAT:
                a = event::WindowKeyboardButton::Action::REPEAT;
                break;
            case GLFW_RELEASE:
                a = event::WindowKeyboardButton::Action::RELEASE;
                break;
        }

        event::WindowKeyboardButton e(key, a);
        event::publish(e);
    });

    glfwSetErrorCallback([](int error, const char* description) { LOG_ERROR("gfx::Window", "GLFW error($0): $1", error, std::string(description)); });

    if (info.useOpenGL)
        glfwMakeContextCurrent(_window);

#ifdef ATTA_OS_WEB
    int w = canvas_get_width();
    int h = canvas_get_height();
    glfwSetWindowSize(_window, w, h);
#endif
}

GlfwWindow::~GlfwWindow() {
    glfwDestroyWindow(_window);

    if (--_glfwWindowCounter == 0)
        glfwTerminate();
}

void* GlfwWindow::getHandle() const { return _window; }

void GlfwWindow::update() {
#ifdef ATTA_OS_WEB
    int w = canvas_get_width();
    int h = canvas_get_height();
    glfwSetWindowSize(_window, w, h);
#endif
    glfwPollEvents();
}

void GlfwWindow::swapBuffers() { glfwSwapBuffers(_window); }

} // namespace atta::graphics
