#include "stbImage.h"
#include "window.h"


Window::Window(WindowConfig config):
	_config(config)
{
	if(!glfwInit())
	{
		std::cout << BOLDRED << "[Window] glfwInit() failed!" << RESET << std::endl;
		exit(1);
	}

	if(!glfwVulkanSupported())
	{
		std::cout << BOLDRED << "[Window] glfwVulkanSupported() failed!" << RESET << std::endl;
		exit(1);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);

	//----- Create Window -----//
	GLFWmonitor* monitor = _config.fullscreen ? glfwGetPrimaryMonitor() : nullptr;

	_window = glfwCreateWindow(_config.width, _config.height, _config.title.c_str(), monitor, nullptr);
	//----- Icon -----//
	GLFWimage icon;
	icon.pixels = stbi_load("assets/textures/icon.png", &icon.width, &icon.height, nullptr, 4);

	if(icon.pixels == nullptr)
	{
		std::cout << BOLDRED << "[Window] Failed to load icon!" << RESET << std::endl;
		exit(1);
	}

	glfwSetWindowIcon(_window, 1, &icon);
	stbi_image_free(icon.pixels);

	//----- Cursor setup -----//
	if(_config.cursorDisabled)
	{
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

Window::~Window()
{
	if(_window != nullptr)
		glfwDestroyWindow(_window);

	glfwTerminate();
}

void Window::close()
{
	glfwSetWindowShouldClose(_window, 1);
}

void Window::run()
{
	glfwSetTime(0.0);

	while(!glfwWindowShouldClose(_window))
	{
		glfwPollEvents();

		if(drawFrame)
		{
			drawFrame();
		}
	}
}

bool Window::isMinimized() const
{
	const auto size = framebufferSize();
	return size.height == 0 && size.width == 0;
}

void Window::waitForEvents() const
{
	glfwWaitEvents();
}

std::vector<const char*> Window::getRequiredExtensions() const
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	return std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
}

VkExtent2D Window::framebufferSize() const
{
	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	return VkExtent2D{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

VkExtent2D Window::windowSize() const
{
	int width, height;
	glfwGetWindowSize(_window, &width, &height);
	return VkExtent2D{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}
