//--------------------------------------------------
// Robot Simulator
// pipelineLayout.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PIPELINE_LAYOUT_H
#define PIPELINE_LAYOUT_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "../device.h"
#include "../descriptorSetLayout.h"
#include "../helpers.h"

class PipelineLayout
{
	public:
		PipelineLayout(std::shared_ptr<Device> device, DescriptorSetLayout* descriptorSetLayout);
		~PipelineLayout();

		VkPipelineLayout handle() const { return _pipelineLayout; }

	private:
		VkPipelineLayout _pipelineLayout;
		std::shared_ptr<Device> _device;
		DescriptorSetLayout* _descriptorSetLayout;
};

#endif// PIPELINE_LAYOUT_H
