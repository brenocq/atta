//--------------------------------------------------
// Robot Simulator
// instance.h
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_INSTANCE_H
#define ATTA_GRAPHICS_VULKAN_INSTANCE_H

#include <atta/graphics/vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <atta/graphics/vulkan/debugCommon.h>

namespace atta::vk
{
	class Instance
	{
		public:
			Instance();
			~Instance();

			VkInstance handle() const { return _instance; }
		private:
			void printExtensionSupport();
			void printLayersProperties();
			std::vector<const char*> getRequiredExtensions();

			bool checkValidationLayerSupport();
			VkInstance _instance;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_INSTANCE_H
