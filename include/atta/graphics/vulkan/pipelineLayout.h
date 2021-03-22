//--------------------------------------------------
// Robot Simulator
// pipelineLayout.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_PIPELINE_LAYOUT_H
#define ATTA_GRAPHICS_VULKAN_PIPELINE_LAYOUT_H

#include <iostream>
#include <string.h>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/descriptorSetLayout.h>
#include <atta/graphics/vulkan/helpers.h>
#include <atta/graphics/core/objectInfo.h>

namespace atta::vk
{
	class PipelineLayout
	{
		public:
			PipelineLayout(std::shared_ptr<Device> device, std::shared_ptr<DescriptorSetLayout> descriptorSetLayout);
			~PipelineLayout();

			VkPipelineLayout handle() const { return _pipelineLayout; }

		private:
			VkPipelineLayout _pipelineLayout;
			std::shared_ptr<Device> _device;
			std::shared_ptr<DescriptorSetLayout> _descriptorSetLayout;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_PIPELINE_LAYOUT_H
