//--------------------------------------------------
// Robot Simulator
// surface.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_SURFACE_H
#define ATTA_GRAPHICS_VULKAN_SURFACE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string.h>
#include <atta/graphics/core/window.h>
#include <atta/graphics/vulkan/instance.h>

namespace atta::vk
{
	class Surface
	{
		public:
			Surface(std::shared_ptr<Instance> instance, std::shared_ptr<Window> window);
			~Surface();

			VkSurfaceKHR handle() const { return _surface; }
			std::shared_ptr<Window> getWindow() { return _window; }

		private:
			VkSurfaceKHR _surface;
			std::shared_ptr<Instance> _instance;
			std::shared_ptr<Window> _window;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_SURFACE_H
