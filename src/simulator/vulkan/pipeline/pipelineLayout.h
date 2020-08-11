//--------------------------------------------------
// Robot Simulator
// pipelineLayout.h
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PIPELINE_LAYOUT_H
#define PIPELINE_LAYOUT_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "../device.h"
#include "../descriptorSetLayout.h"

class PipelineLayout
{
	public:
	PipelineLayout(Device* device, DescriptorSetLayout* descriptorSetLayout);
	~PipelineLayout();

	VkPipelineLayout handle() const { return _pipelineLayout; }

	private:
    VkPipelineLayout _pipelineLayout;
	Device* _device;
	DescriptorSetLayout* _descriptorSetLayout;
};

#endif// PIPELINE_LAYOUT_H
