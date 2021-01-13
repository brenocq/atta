//--------------------------------------------------
// Robot Simulator
// pipelineLayout.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_PIPELINE_LAYOUT_H
#define ATTA_VK_PIPELINE_LAYOUT_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "descriptorSetLayout.h"
#include "helpers.h"

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

#endif// ATTA_VK_PIPELINE_LAYOUT_H
