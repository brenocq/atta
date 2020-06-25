//--------------------------------------------------
// Robot Simulator
// surface.cpp
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "surface.h"

Surface::Surface(Instance* instance, Window* window)
{
	_instance = instance;
	_window = window;

	// Windows
	//VkWin32SurfaceCreateInfoKHR createInfo{};
	//createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	//createInfo.hwnd = glfwGetWin32Window(window->handle());
	//createInfo.hinstance = GetModuleHandle(nullptr);

	//if(vkCreateWin32SurfaceKHR(_instance, &createInfo, nullptr, &_surface) != VK_SUCCESS) 
	//{
	//	std::cout << BOLDRED << "[Surface]" << RESET << RED << " Failed to create window surface!" << RESET << std::endl;
	//	exit(1);
	//}

	// Linux
	if(glfwCreateWindowSurface(_instance->handle(), window->handle(), nullptr, &_surface) != VK_SUCCESS) 
	{
		std::cout << BOLDRED << "[Surface]" << RESET << RED << " Failed to create window surface!" << RESET << std::endl;
		exit(1);
    }	
}

Surface::~Surface()
{
	if(_surface != nullptr)
	{
		vkDestroySurfaceKHR(_instance->handle(), _surface, nullptr);
		_surface = nullptr;
	}
}
