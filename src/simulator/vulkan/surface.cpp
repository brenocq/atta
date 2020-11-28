//--------------------------------------------------
// Robot Simulator
// surface.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "surface.h"
#include "simulator/helpers/log.h"

Surface::Surface(Instance* instance, Window* window)
{
	_instance = instance;
	_window = window;

	if(glfwCreateWindowSurface(_instance->handle(), window->handle(), nullptr, &_surface) != VK_SUCCESS) 
	{
		Log::error("Surface", "Failed to create window surface!");
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
