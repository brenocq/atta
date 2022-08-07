//--------------------------------------------------
// Atta Graphics Module
// glfwWindow.cpp
// Date: 2021-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/events/windowCloseEvent.h>
#include <atta/event/events/windowFocusEvent.h>
#include <atta/event/events/windowKeyboardButtonEvent.h>
#include <atta/event/events/windowMouseButtonEvent.h>
#include <atta/event/events/windowMouseMoveEvent.h>
#include <atta/event/events/windowMouseScrollEvent.h>
#include <atta/event/events/windowResizeEvent.h>
#include <atta/graphics/windows/glfwWindow.h>
#include <glad/glad.h>

#ifdef ATTA_OS_WEB
#include <emscripten.h>
EM_JS(int, canvas_get_width, (), { return Module.canvas.width; });
EM_JS(int, canvas_get_height, (), { return Module.canvas.height; });
#endif

namespace atta::graphics {
unsigned GlfwWindow::_glfwWindowCounter = 0;

GlfwWindow::GlfwWindow(const CreateInfo& info) : Window(info) {
    if (_glfwWindowCounter++ == 0) // XXX
        glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// Needed for apple?

    _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(_window, this);

    glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
        WindowCloseEvent e;
        EventManager::publish(e);
    });

    glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
        GlfwWindow& w = *(GlfwWindow*)glfwGetWindowUserPointer(window);
        w._width = width;
        w._height = height;
        WindowResizeEvent e((size_t)width, (size_t)height);
        EventManager::publish(e);
    });

    glfwSetCursorEnterCallback(_window, [](GLFWwindow* window, int entered) {
        WindowFocusEvent e(entered != 0);
        EventManager::publish(e);
    });

    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowMouseMoveEvent e((float)xPos, (float)yPos);
        EventManager::publish(e);
    });

    glfwSetScrollCallback(_window, [](GLFWwindow* window, double dx, double dy) {
        WindowMouseScrollEvent e((float)dx, (float)dy);
        EventManager::publish(e);
    });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            WindowMouseButtonEvent e(button, WindowMouseButtonEvent::Action::PRESS);
            EventManager::publish(e);
        } else if (action == GLFW_RELEASE) {
            WindowMouseButtonEvent e(button, WindowMouseButtonEvent::Action::RELEASE);
            EventManager::publish(e);
        }
    });

    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowKeyboardButtonEvent::Action a;
        switch (action) {
        case GLFW_PRESS:
            a = WindowKeyboardButtonEvent::Action::PRESS;
            break;
        case GLFW_REPEAT:
            a = WindowKeyboardButtonEvent::Action::REPEAT;
            break;
        case GLFW_RELEASE:
            a = WindowKeyboardButtonEvent::Action::RELEASE;
            break;
        }

        WindowKeyboardButtonEvent e(key, a);
        EventManager::publish(e);
    });

    glfwSetErrorCallback([](int error, const char* description) { LOG_ERROR("Window", "GLFW error($0): $1", error, std::string(description)); });

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
