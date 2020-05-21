#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <functional>
#include "defines.h"

struct WindowConfig
{
	std::string title;
	uint32_t width;
	uint32_t height;
	bool cursorDisabled;
	bool fullscreen;
	bool resizable;
};

class Window
{
	public:
	Window(WindowConfig windowConfig);
	~Window();

	void close();
	void run();
	GLFWwindow* handle() const { return _window; }
		
	std::vector<const char*> getRequiredExtensions() const;
	VkExtent2D windowSize() const;

	std::function<void()> drawFrame;
	private:
	WindowConfig _config;
    GLFWwindow* _window;
};

#endif// WINDOW_H
