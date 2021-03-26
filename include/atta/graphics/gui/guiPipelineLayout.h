//--------------------------------------------------
// Robot Simulator
// guiPipelineLayout.h
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_PIPELINE_LAYOUT_H
#define GUI_PIPELINE_LAYOUT_H

#include <iostream>
#include <string.h>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/descriptorSetLayout.h>

namespace atta
{
	class GuiPipelineLayout
	{
		public:
			GuiPipelineLayout(std::shared_ptr<vk::Device> device, std::shared_ptr<vk::DescriptorSetLayout> descriptorSetLayout);
			~GuiPipelineLayout();

			VkPipelineLayout handle() const { return _pipelineLayout; }

		private:
			VkPipelineLayout _pipelineLayout;
			std::shared_ptr<vk::Device> _device;
			std::shared_ptr<vk::DescriptorSetLayout> _descriptorSetLayout;
	};
}

#endif// GUI_PIPELINE_LAYOUT_H
