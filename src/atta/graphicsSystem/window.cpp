//--------------------------------------------------
// Atta Window
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

namespace atta
{
	unsigned Window::_glfwWindowCounter = 0;

	Window::Window(const CreateInfo& info):
		_title(info.title), _width(info.width), _height(info.height),
		_eventManager(info.eventManager)
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
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			w._eventManager->publish(e);
		});

		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			w._width = width;
			w._height = height;

			WindowResizeEvent e((size_t)width, (size_t)height);
			w._eventManager->publish(e);
		});

		glfwSetCursorEnterCallback(_window, [](GLFWwindow* window, int entered)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowFocusEvent e(entered != 0);
			w._eventManager->publish(e);
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowMouseMoveEvent e((float)xPos, (float)yPos);
			w._eventManager->publish(e);
		});

		glfwSetScrollCallback(_window, [](GLFWwindow* window, double dx, double dy)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowMouseScrollEvent e((float)dx, (float)dy);
			w._eventManager->publish(e);
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);

			if(action == GLFW_PRESS)
			{
				WindowMouseButtonEvent e(button, WindowMouseButtonEvent::Action::PRESS);
				w._eventManager->publish(e);
			}
			else if(action == GLFW_RELEASE)
			{
				WindowMouseButtonEvent e(button, WindowMouseButtonEvent::Action::RELEASE);
				w._eventManager->publish(e);
			}
		});

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);

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
			w._eventManager->publish(e);
		});

		glfwSetErrorCallback([](int error, const char* description){
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
