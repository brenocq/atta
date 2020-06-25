//--------------------------------------------------
// Robot Simulator
// window.h
// Date: 21/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <functional>

class Window
{
	public:
	Window();
	~Window();

	void loop();

    GLFWwindow* handle() const { return _window; }
    VkExtent2D getExtent() const { return {(uint32_t)_width, (uint32_t)_height}; } 

	std::function<void()> drawFrame;
	private:
    GLFWwindow* _window;
	int _height;
	int _width;
};

#endif// WINDOW_H
