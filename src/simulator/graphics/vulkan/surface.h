//--------------------------------------------------
// Robot Simulator
// surface.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_SURFACE_H
#define ATTA_VK_SURFACE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string.h>
#include "defines.h"
#include "simulator/graphics/core/window.h"
#include "instance.h"

namespace atta::vk
{
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
}

#endif// ATTA_VK_SURFACE_H
