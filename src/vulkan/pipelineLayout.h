#ifndef PIPELINE_LAYOUT_H
#define PIPELINE_LAYOUT_H

#include "device.h"
#include "descriptorSetLayout.h"

class PipelineLayout
{
	public:
		PipelineLayout(Device* device, DescriptorSetLayout* descriptorSetLayout);
		~PipelineLayout();

		VkPipelineLayout handle() const { return _pipelineLayout; }

	private:
		Device* _device;
		VkPipelineLayout _pipelineLayout;
};

#endif// PIPELINE_LAYOUT_H
