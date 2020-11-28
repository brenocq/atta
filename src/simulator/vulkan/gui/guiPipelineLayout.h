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
#include "defines.h"
#include "simulator/vulkan/device.h"
#include "simulator/vulkan/descriptorSetLayout.h"

class GuiPipelineLayout
{
	public:
		GuiPipelineLayout(Device* device, DescriptorSetLayout* descriptorSetLayout);
		~GuiPipelineLayout();

		VkPipelineLayout handle() const { return _pipelineLayout; }

	private:
		VkPipelineLayout _pipelineLayout;
		Device* _device;
		DescriptorSetLayout* _descriptorSetLayout;
};

#endif// GUI_PIPELINE_LAYOUT_H
