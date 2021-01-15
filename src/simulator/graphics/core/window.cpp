//--------------------------------------------------
// Atta Graphics
// window.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "defines.h"
#include "simulator/helpers/log.h"

namespace atta
{
	Window::Window():
		_width(1200), _height(900), _cursorVisible(true), _lastX(0), _lastY(0)
	{
		glfwInit();
		// To not create an OpenGL context
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// Set window as not resizable
		// TODO fix recreate swapchain errors
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		//----- Create Window -----//
		_window = glfwCreateWindow(_width, _height, "Atta Robot Simulator - by Brenocq", nullptr, nullptr);
		glfwSetWindowUserPointer(_window, this);
		glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);

		//----- Icon -----//
		GLFWimage icon;
		icon.pixels = stbi_load("assets/textures/icon.png", &icon.width, &icon.height, nullptr, 4);

		if(icon.pixels == nullptr)
		{
			Log::error("Window", "Failed to load icon!");
			exit(1);
		}

		glfwSetWindowIcon(_window, 1, &icon);
		stbi_image_free(icon.pixels);

		//----- Callbacks -----//
		glfwSetWindowUserPointer(_window, this);
		glfwSetKeyCallback(_window, keyCallback);
		glfwSetCursorPosCallback(_window, cursorPositionCallback);
		glfwSetMouseButtonCallback(_window, mouseButtonCallback);
		glfwSetScrollCallback(_window, scrollCallback);

		//----- Window config -----//
		glfwSetInputMode(_window, GLFW_CURSOR, _cursorVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED );
	}

	Window::~Window()
	{
		// Destroy the window and terminate glfw
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void Window::loop()
	{
		// Run in loop until the window is closed
		while(!glfwWindowShouldClose(_window)) 
		{
			glfwPollEvents();

			if(drawFrame)
				drawFrame();
		}
	}

	VkExtent2D Window::getExtent()
	{ 
		glfwGetFramebufferSize(_window, &_width, &_height);

		return {(uint32_t)_width, (uint32_t)_height}; 
	} 

	void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		Window* win = (Window*) glfwGetWindowUserPointer(window);
		if(win->windowResized)
		{
			win->windowResized();
		}
	}

	void Window::waitIfMinimized()
	{
		// Wait until window isnt minimized
		glfwGetFramebufferSize(_window, &_width, &_height);
		while(_width == 0 || _height == 0) 
		{
			glfwGetFramebufferSize(_window, &_width, &_height);
			glfwWaitEvents();
		}
	}

	double Window::getTime()
	{
		return glfwGetTime();
	}

	void Window::close()
	{
		glfwSetWindowShouldClose(_window, 1);
	}

	void Window::toggleCursorVisibility()
	{
		_cursorVisible = !_cursorVisible;
		if(_cursorVisible)
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	//------------------------------//
	//--------- Callbacks ----------//
	//------------------------------//
	void Window::keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
	{
		//Log::debug("Window", "key -> key: [w]$0[], scancode: [w]$1[], action: [w]$2[], mods: [w]$3[]", key, scancode, action, mods);
		Window* const _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(_this->onKey)
		{
			_this->onKey(key, scancode, action, mods);
		}
	}

	void Window::cursorPositionCallback(GLFWwindow* window, const double xpos, const double ypos)
	{
		//Log::debug("Window", "cursorPosition -> xpos: [w]$0[], ypos: [w]$1[]", xpos, ypos);
		Window* const _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(_this->getCursorVisible())
		{
			_this->setLastX(xpos);
			_this->setLastY(ypos);
		}

		if(_this->onCursorPosition)
		{
			_this->onCursorPosition(xpos, ypos);
		}
	}

	void Window::mouseButtonCallback(GLFWwindow* window, const int button, const int action, const int mods)
	{
		//Log::debug("Window", "mouseButton -> button: [w]$0[], action: [w]$1[], mods: [w]$2[]", button, action, mods);
		Window* const _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(_this->onMouseButton)
		{
			_this->onMouseButton(button, action, mods);
		}
	}

	void Window::scrollCallback(GLFWwindow* window, const double xoffset, const double yoffset)
	{
		//Log::debug("Window", "scrool -> xoffset: [w]$0[], yoffset: [w]$1[]", xoffset, yoffset);
		Window* const _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(_this->onScroll)
		{
			_this->onScroll(xoffset, yoffset);
		}
	}
}