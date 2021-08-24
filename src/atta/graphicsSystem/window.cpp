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
		_eventDispatcher(info.eventDispatcher)
	{
		if(_glfwWindowCounter++ == 0)
			glfwInit();

		_window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(_window, this);

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			w._eventDispatcher->publish(e);
		});

		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			w._width = width;
			w._height = height;

			WindowResizeEvent e((size_t)width, (size_t)height);
			w._eventDispatcher->publish(e);
		});

		glfwSetCursorEnterCallback(_window, [](GLFWwindow* window, int entered)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowFocusEvent e(entered != 0);
			w._eventDispatcher->publish(e);
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowMouseMoveEvent e((float)xPos, (float)yPos);
			w._eventDispatcher->publish(e);
		});

		glfwSetScrollCallback(_window, [](GLFWwindow* window, double dx, double dy)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowMouseScrollEvent e((float)dx, (float)dy);
			w._eventDispatcher->publish(e);
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);

			if(action == GLFW_PRESS)
			{
				WindowMouseButtonEvent e(button, WindowMouseButtonEvent::Action::PRESS);
				w._eventDispatcher->publish(e);
			}
			else if(action == GLFW_RELEASE)
			{
				WindowMouseButtonEvent e(button, WindowMouseButtonEvent::Action::RELEASE);
				w._eventDispatcher->publish(e);
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
			w._eventDispatcher->publish(e);
		});

		glfwSetErrorCallback([](int error, const char* description){
			LOG_ERROR("Window", "GLFW error($0): $1", error, std::string(description));
		});


		// XXX Only testing
		glfwMakeContextCurrent(_window);
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Window", "Failed to initialize GLAD");
			return;
		}

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
		// XXX Only testing
		glClearColor(0.3, 0.5, 0.8, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(_window);
	}
}
