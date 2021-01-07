//--------------------------------------------------
// Robot Simulator
// surface.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SURFACE_H
#define SURFACE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string.h>
#include "defines.h"
#include "window.h"
#include "instance.h"

class Surface
{
	public:
	Surface(std::shared_ptr<Instance> instance, std::shared_ptr<Window> window);
	~Surface();

	VkSurfaceKHR handle() const { return _surface; }
	private:

    VkSurfaceKHR _surface;
	std::shared_ptr<Instance> _instance;
	std::shared_ptr<Window> _window;
};

#endif// SURFACE_H
