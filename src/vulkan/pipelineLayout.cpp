#include "pipelineLayout.h"

PipelineLayout::PipelineLayout(Device* device/*, DescriptorSetLayout* descriptorSetLayout*/):
	_device(device)
{
	//VkDescriptorSetLayout descriptorSetLayouts[] = { descriptorSetLayout->handle() };

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;//1;
	pipelineLayoutInfo.pSetLayouts = nullptr;//descriptorSetLayouts;
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if(vkCreatePipelineLayout(_device->handle(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[PipelineLayout] Failed to create pipeline layout!";
		exit(1);
	}
}

PipelineLayout::~PipelineLayout()
{
	if(_pipelineLayout != nullptr)
		vkDestroyPipelineLayout(_device->handle(), _pipelineLayout, nullptr);
}
