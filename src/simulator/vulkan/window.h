//--------------------------------------------------
// Robot Simulator
// window.h
// Date: 2020-06-21
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

		// Callbacks
		std::function<void()> drawFrame;
		std::function<void()> windowResized;

		// Glfw callbacks
		std::function<void(int key, int scancode, int action, int mods)> onKey;
		std::function<void(double xpos, double ypos)> onCursorPosition;
		std::function<void(int button, int action, int mods)> onMouseButton;
		std::function<void(double xoffset, double yoffset)> onScroll;

		double getTime();
		void close();
		void toggleCursorVisibility();
	private:
		static void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods);
		static void cursorPositionCallback(GLFWwindow* window, const double xpos, const double ypos);
		static void mouseButtonCallback(GLFWwindow* window, const int button, const int action, const int mods);
		static void scrollCallback(GLFWwindow* window, const double xoffset, const double yoffset);

		GLFWwindow* _window;
		int _height;
		int _width;
		bool _cursorVisible;
};

#endif// WINDOW_H
