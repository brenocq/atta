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
    VkExtent2D getExtent();
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	void waitIfMinimized();

	std::function<void()> drawFrame;
	std::function<void()> windowResized;
	private:
    GLFWwindow* _window;
	int _height;
	int _width;
};

#endif// WINDOW_H
