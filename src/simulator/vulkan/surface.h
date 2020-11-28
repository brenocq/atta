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
#include <string.h>
#include "defines.h"
#include "window.h"
#include "instance.h"

class Surface
{
	public:
	Surface(Instance* instance, Window* window);
	~Surface();

	VkSurfaceKHR handle() const { return _surface; }
	private:

    VkSurfaceKHR _surface;
	Window* _window;
	Instance* _instance;
};

#endif// SURFACE_H
