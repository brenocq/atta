//--------------------------------------------------
// Robot Simulator
// window.cpp
// Date: 21/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "window.h"

Window::Window():
	_width(800), _height(600)
{
	glfwInit();
	// To not create an OpenGL context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// Set window as not resizable
	// TODO fix recreate swapchain errors
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// Create window
	_window = glfwCreateWindow(_width, _height, "Robot Simulator - by Brenocq", nullptr, nullptr);
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
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
		{
			drawFrame();
		}
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
