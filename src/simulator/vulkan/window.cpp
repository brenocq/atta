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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// Create window
	_window = glfwCreateWindow(_width, _height, "Robot Simulator - by Brenocq", nullptr, nullptr);

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
