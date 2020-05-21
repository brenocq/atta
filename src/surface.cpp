#include "surface.h"

Surface::Surface(Instance* instance):
	_instance(instance)
{
	if(glfwCreateWindowSurface(instance->handle(), instance->window()->handle(), nullptr, &_surface) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Surface] Failed to create surface!" << RESET << std::endl;
		exit(1);
	}
}

Surface::~Surface()
{
	if (_surface != nullptr)
		vkDestroySurfaceKHR(_instance->handle(), _surface, nullptr);
}
