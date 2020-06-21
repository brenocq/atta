#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

class Window
{
	public:
	Window();
	~Window();

	void loop();

	private:
    GLFWwindow* _window;
	int _height;
	int _width;
};

#endif// WINDOW_H
