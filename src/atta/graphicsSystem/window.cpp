//--------------------------------------------------
// Atta Graphics System
// window.cpp
// Date: 2021-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/window.h>
#include <atta/eventSystem/events/windowCloseEvent.h>
#include <atta/eventSystem/events/windowResizeEvent.h>
#include <atta/eventSystem/events/windowFocusEvent.h>
#include <atta/eventSystem/events/windowMouseMoveEvent.h>
#include <atta/eventSystem/events/windowMouseScrollEvent.h>
#include <atta/eventSystem/events/windowMouseButtonEvent.h>
#include <atta/eventSystem/events/windowKeyboardButtonEvent.h>
#include <glad/glad.h>

namespace atta
{
    unsigned Window::_glfwWindowCounter = 0;

    Window::Window(const CreateInfo& info):
        _title(info.title), _width(info.width), _height(info.height)
    {
        if(_glfwWindowCounter++ == 0)// XXX
            glfwInit();

        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// Needed for apple?

        _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(_window, this);

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
            {
                WindowCloseEvent e;
                EventManager::publish(e);
            });

        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
            {
                Window& w = *(Window*)glfwGetWindowUserPointer(window);
                w._width = width;
                w._height = height;

                WindowResizeEvent e((size_t)width, (size_t)height);
                EventManager::publish(e);
            });

        glfwSetCursorEnterCallback(_window, [](GLFWwindow* window, int entered)
            {
                WindowFocusEvent e(entered != 0);
                EventManager::publish(e);
            });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowMouseMoveEvent e((float)xPos, (float)yPos);
                EventManager::publish(e);
            });

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double dx, double dy)
            {
                WindowMouseScrollEvent e((float)dx, (float)dy);
                EventManager::publish(e);
            });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
            {
                if(action == GLFW_PRESS)
                {
                    WindowMouseButtonEvent e(button, WindowMouseButtonEvent::Action::PRESS);
                    EventManager::publish(e);
                }
                else if(action == GLFW_RELEASE)
                {
                    WindowMouseButtonEvent e(button, WindowMouseButtonEvent::Action::RELEASE);
                    EventManager::publish(e);
                }
            });

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowKeyboardButtonEvent::Action a;
                switch(action)
                {
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

        glfwSetErrorCallback([](int error, const char* description)
            {
                LOG_ERROR("Window", "GLFW error($0): $1", error, std::string(description));
            });

        glfwMakeContextCurrent(_window);
        LOG_VERBOSE("Window", "Window created");
    }

    Window::~Window()
    {
        glfwDestroyWindow(_window);

        if(--_glfwWindowCounter == 0)
            glfwTerminate();
    }

    void Window::update()
    {
        glfwPollEvents();
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(_window);
    }
}
